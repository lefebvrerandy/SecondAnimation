/****************************************************************************************** 
 *	Chili DirectX Framework Sound Pack Version 16.11.11									  *	
 *	Sound.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with this source code.  If not, see <http://www.gnu.org/licenses/>.			  *
 ******************************************************************************************/
#include "Sound.h"
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <array>
#include <functional>
#include "XAudio\XAudio2.h"

SoundSystem& SoundSystem::Get()
{
	static SoundSystem instance;
	return instance;
}

const WAVEFORMATEX& SoundSystem::GetFormat()
{
	return *Get().format;
}

void SoundSystem::PlaySoundBuffer( Sound & s,float freqMod,float vol )
{
	std::lock_guard<std::mutex> lock( mutex );
	if( idleChannelPtrs.size() > 0 )
	{
		activeChannelPtrs.push_back( std::move( idleChannelPtrs.back() ) );
		idleChannelPtrs.pop_back();
		activeChannelPtrs.back()->PlaySoundBuffer( s,freqMod,vol );
	}
}

SoundSystem::XAudioDll::XAudioDll()
{
	LoadType type = LoadType::System;
	while( true )
	{
		hModule = LoadLibrary((LPCSTR)GetDllPath( type ) );
		if( hModule != 0 )
		{
			return;
		}
		else
		{
			switch( type )
			{
			case LoadType::System:
				type = LoadType::Folder;
				break;
			case LoadType::Folder:
				type = LoadType::Local;
				break;
			case LoadType::Local:
				return;
			default:
				assert( false && "Bad LoadType encountered in XAudio Dll loading sequence loop" );
			}
		}
	}
}

SoundSystem::XAudioDll::~XAudioDll()
{
	if( hModule != 0 )
	{
		FreeLibrary( hModule );
		hModule = 0;
	}
}

SoundSystem::XAudioDll::operator HMODULE() const
{
	return hModule;
}

const wchar_t* SoundSystem::XAudioDll::GetDllPath( LoadType type )
{
	switch( type )
	{
	case LoadType::System:
		return systemPath;
	case LoadType::Folder:
		return folderPath;
	case LoadType::Local:
		return localPath;
	default:
		assert( false && "Bad LoadType in GetDllPath function" );
		return nullptr;
	}
}

SoundSystem::SoundSystem()
	:
	format( std::make_unique<WAVEFORMATEX>() )
{
	// setup wave format info structure
	static_assert(nChannelsPerSound > 0u,"WAVE File Format Error: At least one channel required per audio stream");
	static_assert(nChannelsPerSound <= XAUDIO2_MAX_AUDIO_CHANNELS,"WAVE File Format Error: Maximum audio channels per audio stream exceeded");
	static_assert(nSamplesPerSec >= XAUDIO2_MIN_SAMPLE_RATE,"WAVE File Format Error: Sample rate lower than minimum allowed");
	static_assert(nSamplesPerSec <= XAUDIO2_MAX_SAMPLE_RATE,"WAVE File Format Error: Sample rate exceeds maximum allowed");
	static_assert(nBitsPerSample > 0u,"WAVE File Format Error: Bit depth of 0 bits per sample is not allowed");
	static_assert(nBitsPerSample % 8u == 0,"WAVE File Format Error: Bit depth must be multiple of 8");
	format->nChannels = nChannelsPerSound;
	format->nSamplesPerSec = nSamplesPerSec;
	format->wBitsPerSample = nBitsPerSample;
	format->nBlockAlign = (nBitsPerSample / 8) * nChannelsPerSound;
	format->nAvgBytesPerSec = format->nBlockAlign * nSamplesPerSec;
	format->cbSize = 0;
	format->wFormatTag = WAVE_FORMAT_PCM;
	
	// find address of DllGetClassObject() function in the dll
	const std::function<HRESULT(REFCLSID,REFIID,LPVOID)> DllGetClassObject =
        reinterpret_cast<HRESULT(WINAPI*)(REFCLSID,REFIID,LPVOID)>( 
		GetProcAddress( xaudio_dll,"DllGetClassObject" ) );
	if( !DllGetClassObject )
	{		
		return;
	}

	// create the factory class for the XAudio2 component object
	Microsoft::WRL::ComPtr<IClassFactory> pClassFactory;
	HRESULT hr;
	if( FAILED( hr = DllGetClassObject( 
		 __uuidof( XAudio2 ),
		IID_IClassFactory,
		pClassFactory.ReleaseAndGetAddressOf() ) ) )
	{
		return;
	}

	// create the XAudio2 component object itself
	if( FAILED( hr = pClassFactory->CreateInstance( nullptr,
		__uuidof( IXAudio2 ),&pEngine ) ) )
	{
		return;
	}

	// initialize the XAudio2 component object
	if( FAILED( hr = pEngine->Initialize( 0,XAUDIO2_DEFAULT_PROCESSOR ) ) )
	{
		return;
	}

	// create the mastering voice
	if( FAILED( hr = pEngine->CreateMasteringVoice( &pMaster ) ) )
	{
		return;
	}

	// create channel objects
	for( int i = 0; i < nChannels; i++ )
	{
		idleChannelPtrs.push_back( std::make_unique<Channel>( *this ) );
	}
}

void SoundSystem::DeactivateChannel( Channel & channel )
{
	std::lock_guard<std::mutex> lock( mutex );
	auto i = std::find_if( activeChannelPtrs.begin(),activeChannelPtrs.end(),
		[&channel]( const std::unique_ptr<Channel>& pChan ) -> bool
	{
		return &channel == pChan.get();
	} );
	idleChannelPtrs.push_back( std::move( *i ) );
	activeChannelPtrs.erase( i );
}

SoundSystem::Channel::Channel( SoundSystem & sys )
	:
	xaBuffer( std::make_unique<XAUDIO2_BUFFER>() )
{
	class VoiceCallback : public IXAudio2VoiceCallback
	{
	public:
		void STDMETHODCALLTYPE OnStreamEnd() override
		{}
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override
		{}
		void STDMETHODCALLTYPE OnVoiceProcessingPassStart( UINT32 SamplesRequired ) override
		{}
		void STDMETHODCALLTYPE OnBufferEnd( void* pBufferContext ) override
		{
			Channel& chan = *reinterpret_cast<Channel*>( pBufferContext );
			chan.Stop();
			{
				std::lock_guard<std::mutex> lock( chan.pSound->mutex );
				chan.pSound->activeChannelPtrs.erase( std::find(
					chan.pSound->activeChannelPtrs.begin(),chan.pSound->activeChannelPtrs.end(),&chan ) );
				// notify any thread that might be waiting for activeChannels
				// to become zero (i.e. thread calling destructor)
				chan.pSound->cvDeath.notify_all();
			}
			chan.pSound = nullptr;
			SoundSystem::Get().DeactivateChannel( chan );
		}
		void STDMETHODCALLTYPE OnBufferStart( void* pBufferContext ) override
		{}
		void STDMETHODCALLTYPE OnLoopEnd( void* pBufferContext ) override
		{}
		void STDMETHODCALLTYPE OnVoiceError( void* pBufferContext,HRESULT Error ) override
		{}
	};
	static VoiceCallback vcb;
	ZeroMemory( xaBuffer.get(),sizeof( *xaBuffer ) );
	xaBuffer->pContext = this;
	HRESULT hr;
	if( FAILED( hr = sys.pEngine->CreateSourceVoice( &pSource,sys.format.get(),0u,2.0f,&vcb ) ) )
	{
		return;
	}
}

SoundSystem::Channel::~Channel()
{
	assert( !pSound );
	if( pSource )
	{
		pSource->DestroyVoice();
		pSource = nullptr;
	}
}

void SoundSystem::Channel::PlaySoundBuffer( Sound& s,float freqMod,float vol )
{
	assert( pSource && !pSound );
	{
		std::lock_guard<std::mutex> lock( s.mutex );
		s.activeChannelPtrs.push_back( this );
	}
	// callback thread not running yet, so no sync necessary for pSound
	pSound = &s;
	xaBuffer->pAudioData = s.pData.get();
	xaBuffer->AudioBytes = s.nBytes;
	if( s.looping )
	{
		xaBuffer->LoopBegin = s.loopStart;
		xaBuffer->LoopLength = s.loopEnd - s.loopStart;
		xaBuffer->LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		xaBuffer->LoopCount = 0u;
	}
	HRESULT hr;
	if( FAILED( hr = pSource->SubmitSourceBuffer( xaBuffer.get(),nullptr ) ) )
	{
		return;
	}
	if( FAILED( hr = pSource->SetFrequencyRatio( freqMod ) ) )
	{
		return;
	}
	if( FAILED( hr = pSource->SetVolume( vol ) ) )
	{
		return;
	}
	if( FAILED( hr = pSource->Start() ) )
	{
		return;
	}
}

void SoundSystem::Channel::Stop()
{
	assert( pSource && pSound );
	pSource->Stop();
	pSource->FlushSourceBuffers();
}

void SoundSystem::Channel::RetargetSound( const Sound* pOld,Sound* pNew )
{
	assert( pOld == pSound );
	pSound = pNew;
}

Sound::Sound()
	:
	looping( false ),
	nBytes( 0u )
{}

Sound::Sound( const std::wstring & fileName,bool loopingWithAutoDetect )
	:
	Sound( fileName,loopingWithAutoDetect,false,nullSample,nullSample,nullSeconds,nullSeconds )
{
}

Sound::Sound( const std::wstring & fileName,unsigned int loopStart,unsigned int loopEnd )
	:
	Sound( fileName,false,true,loopStart,loopEnd,nullSeconds,nullSeconds )
{
}

Sound::Sound( const std::wstring & fileName,float loopStart,float loopEnd )
	:
	Sound( fileName,false,true,nullSample,nullSample,loopStart,loopEnd )
{

}

Sound::Sound( const std::wstring & fileName,bool detectLooping,bool manualLooping,
	unsigned int loopStartSample,unsigned int loopEndSample,
	float loopStartSeconds,float loopEndSeconds )
	:
	looping( detectLooping || manualLooping )
{
	// check if manual and detect settings are consistent
	assert( !(detectLooping && manualLooping) );
	// check if manual loop points are consistent with each other and with manual loop switch
	assert( manualLooping == ((loopStartSample == nullSample && loopEndSample == nullSample) !=
		(loopStartSeconds == nullSeconds && loopEndSeconds == nullSeconds)) );
	
	unsigned int fileSize = 0;
	std::unique_ptr<BYTE[]> pFileIn;
	try
	{
		{
			std::ifstream file;
			file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
			file.open( fileName,std::ios::binary );

			{
				int fourcc;
				file.read( reinterpret_cast<char*>(&fourcc),4 );
				if( fourcc != 'FFIR' )
				{
					return;
				}
			}

			file.read( reinterpret_cast<char*>(&fileSize),4 );
			fileSize += 8; // entry doesn't include the fourcc or itself
			if( fileSize <= 44 )
			{
				return;
			}

			file.seekg( 0,std::ios::beg );
			pFileIn = std::make_unique<BYTE[]>( fileSize );
			file.read( reinterpret_cast<char*>(pFileIn.get()),fileSize );
		}

		if( *reinterpret_cast<const unsigned int*>(&pFileIn[8]) != 'EVAW' )
		{
			return;
		}

		//look for 'fmt ' chunk id
		WAVEFORMATEX format;
		bool bFilledFormat = false;
		for( unsigned int i = 12; i < fileSize; )
		{
			if( *reinterpret_cast<const unsigned int*>(&pFileIn[i]) == ' tmf' )
			{
				memcpy( &format,&pFileIn[i + 8],sizeof( format ) );
				bFilledFormat = true;
				break;
			}
			// chunk size + size entry size + chunk id entry size + word padding
			i += (*reinterpret_cast<const unsigned int*>(&pFileIn[i + 4]) + 9) & 0xFFFFFFFE;
		}
		if( !bFilledFormat )
		{
			return;
		}

		// compare format with sound system format
		{
			const WAVEFORMATEX& sysFormat = SoundSystem::GetFormat();

			if( format.nChannels != sysFormat.nChannels )
			{
				return;
			}
			else if( format.wBitsPerSample != sysFormat.wBitsPerSample )
			{
				return;
			}
			else if( format.nSamplesPerSec != sysFormat.nSamplesPerSec )
			{
				return;
			}
			else if( format.wFormatTag != sysFormat.wFormatTag )
			{
				return;
			}
			else if( format.nBlockAlign != sysFormat.nBlockAlign )
			{
				return;
			}
			else if( format.nAvgBytesPerSec != sysFormat.nAvgBytesPerSec )
			{
				return;
			}
		}

		//look for 'data' chunk id
		bool bFilledData = false;
		for( unsigned int i = 12; i < fileSize; )
		{
			const int chunkSize = *reinterpret_cast<const unsigned int*>(&pFileIn[i + 4]);
			if( *reinterpret_cast<const unsigned int*>(&pFileIn[i]) == 'atad' )
			{
				pData = std::make_unique<BYTE[]>( chunkSize );
				nBytes = chunkSize;
				memcpy( pData.get(),&pFileIn[i + 8],nBytes );

				bFilledData = true;
				break;
			}
			// chunk size + size entry size + chunk id entry size + word padding
			i += (chunkSize + 9) & 0xFFFFFFFE;
		}
		if( !bFilledData )
		{
			return;
		}

		if( detectLooping )
		{
			//look for 'cue' chunk id
			bool bFilledCue = false;
			for( unsigned int i = 12; i < fileSize; )
			{
				const int chunkSize = *reinterpret_cast<const unsigned int*>(&pFileIn[i + 4]);
				if( *reinterpret_cast<const unsigned int*>(&pFileIn[i]) == ' euc' )
				{
					struct CuePoint
					{
						unsigned int cuePtId;
						unsigned int pop;
						unsigned int dataChunkId;
						unsigned int chunkStart;
						unsigned int blockStart;
						unsigned int frameOffset;
					};

					const unsigned int nCuePts =
						*reinterpret_cast<const unsigned int*>(&pFileIn[i + 8]);
					if( nCuePts != 2 )
					{
						continue;
					}

					const CuePoint* const pCuePts =
						reinterpret_cast<const CuePoint* const>(&pFileIn[i + 12]);
					loopStart = pCuePts[0].frameOffset;
					loopEnd = pCuePts[1].frameOffset;
					bFilledCue = true;
					break;
				}
				// chunk size + size entry size + chunk id entry size + word padding
				i += (chunkSize + 9) & 0xFFFFFFFE;
			}
			if( !bFilledCue )
			{
				return;
			}
		}
		else if( manualLooping )
		{
			const WAVEFORMATEX& sysFormat = SoundSystem::GetFormat();
			const unsigned int nFrames = nBytes / sysFormat.nBlockAlign;
			if( loopStartSeconds != -1.0f )
			{
				const unsigned int nFramesPerSec = sysFormat.nAvgBytesPerSec / sysFormat.nBlockAlign;
				loopStart = unsigned int( loopStartSeconds * float( nFramesPerSec ) );
				assert( loopStart < nFrames );
				loopEnd = unsigned int( loopEndSeconds * float( nFramesPerSec ) );
				assert( loopEnd > loopStart && loopEnd < nFrames );
			}
			else
			{
				assert( loopStartSample < nFrames );
				loopStart = loopStartSample;
				assert( loopEndSample > loopStartSample && loopEndSample < nFrames );
				loopEnd = loopEndSample;
			}
			// just in case ;)
			loopStart = min( loopStart,nFrames - 1 );
			loopEnd = min( loopEnd,nFrames - 1 );
		}
	}
	catch( const std::exception& e )
	{
		nBytes = 0u;
		looping = false;
		pData.release();
		// needed for conversion to wide string
		const std::string what = e.what();
		return;
	}
}

Sound::Sound( Sound&& donor )
{
	std::lock_guard<std::mutex> lock( donor.mutex );
	nBytes = donor.nBytes;
	donor.nBytes = 0u;
	looping = donor.looping;
	loopStart = donor.loopStart;
	loopEnd = donor.loopEnd;
	pData = std::move( donor.pData );
	activeChannelPtrs = std::move( donor.activeChannelPtrs );
	for( auto& pChan : activeChannelPtrs )
	{
		pChan->RetargetSound( &donor,this );
	}
	donor.cvDeath.notify_all();
}

Sound& Sound::operator=( Sound && donor )
{	
	// make sure nobody messes with our shit (also needed for cv.wait())
	std::unique_lock<std::mutex> lock( mutex );
	// check if there are even any active channels playing our jam
	if( activeChannelPtrs.size() != 0u )
	{
		// stop all channels currently playing our jam
		for( auto pChannel : activeChannelPtrs )
		{
			pChannel->Stop();
		}
		// wait for those channels to actually stop playing our jam
		cvDeath.wait( lock,[this] { return activeChannelPtrs.size() == 0u; } );
	}

	std::lock_guard<std::mutex> lock_donor( donor.mutex );
	nBytes = donor.nBytes;
	donor.nBytes = 0u;
	looping = donor.looping;
	loopStart = donor.loopStart;
	loopEnd = donor.loopEnd;
	pData = std::move( donor.pData );
	activeChannelPtrs = std::move( donor.activeChannelPtrs );	
	for( auto& pChan : activeChannelPtrs )
	{
		pChan->RetargetSound( &donor,this );
	}
	donor.cvDeath.notify_all();
	return *this;
}

void Sound::Play( float freqMod,float vol )
{
	SoundSystem::Get().PlaySoundBuffer( *this,freqMod,vol );
}

void Sound::StopOne()
{
	std::lock_guard<std::mutex> lock( mutex );
	if( activeChannelPtrs.size() > 0u )
	{
		activeChannelPtrs.front()->Stop();
	}
}

void Sound::StopAll()
{
	std::lock_guard<std::mutex> lock( mutex );
	for( auto pChannel : activeChannelPtrs )
	{
		pChannel->Stop();
	}
}

Sound::~Sound()
{
	// make sure nobody messes with our shit (also needed for cv.wait())
	std::unique_lock<std::mutex> lock( mutex );

	// check if there are even any active channels playing our jam
	if( activeChannelPtrs.size() == 0u )
	{
		return;
	}

	// stop all channels currently playing our jam
	for( auto pChannel : activeChannelPtrs )
	{
		pChannel->Stop();
	}

	// wait for those channels to actually stop playing our jam
	cvDeath.wait( lock,[this] { return activeChannelPtrs.size() == 0u; } );
}