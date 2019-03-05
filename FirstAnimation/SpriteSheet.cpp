#include "SpriteSheet.h"

/****************************************************
The concept behind this class is that it will be passed
a filename and graphics object/rendertarget, then, will
proceed to create the needed WIC components to read, 
decode, and then encode the bitmap file from disk into
a compatible D2D bitmap. 

We need this approach to be able to address pretty much
any bitmap from disk/resources into the game and use it
within Directx (D2D specifically for now)

*******************************************************/


SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx)
{
	this->gfx = gfx; //save the gfx parameter for later
	bmp = NULL; //This needs to be NULL to start off
	HRESULT hr;

	//Step 1: Create a WIC Factory
	IWICImagingFactory *wicFactory = NULL;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory, //CLS ID of the object about to be made
		NULL, //not needed here, but just denotes this isn't part of an aggregate
		CLSCTX_INPROC_SERVER, //Indicates this DLL runs in the same process
		IID_IWICImagingFactory, //Reference to an Interface that talks to the object
		(LPVOID*)&wicFactory); //This is our pointer to the WICFactory, once set up.

	if (wicFactory == nullptr) return;

//Step 2: Create a Decoder to read file into a WIC Bitmap
	IWICBitmapDecoder *wicDecoder = NULL;
	hr = wicFactory->CreateDecoderFromFilename(
		filename, //The filename we passed in already
		NULL, //This can be used to indicate other/preferred decoders. Not something we need.
		GENERIC_READ, //indicates we're reading from the file, vs writing, etc.
		WICDecodeMetadataCacheOnLoad, //Needed, but would only help if we were keeping this in WIC
		&wicDecoder); //Our pointer to the Decoder we've setup

//Step 3: Read a 'frame'. We're really just moving the whole image into a frame here
	IWICBitmapFrameDecode* wicFrame = NULL;
	hr = wicDecoder->GetFrame(0, &wicFrame); //0 here means the first frame... or only one in our case
	//Now, we've got a WICBitmap... we want it to be a D2D bitmap

//Step 4: Create a WIC Converter
	IWICFormatConverter *wicConverter = NULL;
	hr = wicFactory->CreateFormatConverter(&wicConverter);

//Step 5: Configure the Converter
	hr = wicConverter->Initialize(
		wicFrame, //Our frame from above
		GUID_WICPixelFormat32bppPBGRA, //Pixelformat
		WICBitmapDitherTypeNone, //not important for us here
		NULL, //indicates no palette is needed, not important here
		0.0, //Alpha Transparency, can look at this later
		WICBitmapPaletteTypeCustom //Not important for us here
		);

//Step 6: Create the D2D Bitmap! Finally!
	gfx->GetRenderTarget()->CreateBitmapFromWicBitmap(
		wicConverter, //Our friend the converter
		NULL, //Can specify D2D1_Bitmap_Properties here, not needed now
		&bmp //Our destination bmp we specified earlier in the header
	);
	
	//Let us do some private object cleanup!
	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();
}

SpriteSheet::~SpriteSheet()
{
	if (bmp) bmp->Release();
}

void SpriteSheet::DrawChroma(float x, float y)
{

	ID2D1Effect *chromakeyEffect = NULL;
	ID2D1Effect *scaleEffect = NULL;

	// Create an effect for scale the size of the incoming image
	gfx->GetDeviceContext()->CreateEffect(CLSID_D2D1Scale, &scaleEffect);
	scaleEffect->SetInput(0, bmp);
	D2D1_VECTOR_2F vector1{ PROP_CENTER, PROP_CENTER };
	D2D1_VECTOR_2F vector2{ PROP_SCALE, PROP_SCALE };
	scaleEffect->SetValue(D2D1_SCALE_PROP_CENTER_POINT, vector1);
	scaleEffect->SetValue(D2D1_SCALE_PROP_SCALE, vector2);

	//  Create an effect to handle the chromakey
	D2D1_VECTOR_3F vector{ 0.0f, 1.0f, 0.0f };
	gfx->GetDeviceContext()->CreateEffect(CLSID_D2D1ChromaKey, &chromakeyEffect);

	chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_COLOR, vector);
	chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_TOLERANCE, 0.7f);
	chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_INVERT_ALPHA, false);
	chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_FEATHER, false);

	// Combine the two effects
	chromakeyEffect->SetInputEffect(0, scaleEffect);
	chromakeyEffect->SetInput(1, bmp);
	
	// Display the image
	gfx->GetDeviceContext()->DrawImage(chromakeyEffect, D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y)));


	if (scaleEffect) scaleEffect->Release();
	if (chromakeyEffect) chromakeyEffect->Release();
}

void SpriteSheet::Draw()
{
	gfx->GetRenderTarget()->DrawBitmap(
		bmp, //Bitmap we built from WIC
		D2D1::RectF(0.0f, 0.0f, RESOLUTION_X, RESOLUTION_Y), //Destination rectangle
		0.8f, //Opacity or Alpha
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		//Above - the interpolation mode to use if this object is 'stretched' or 'shrunk'.
		//Refer back to lecture notes on image/bitmap files
		D2D1::RectF(0.0f, 0.0f, RESOLUTION_X, RESOLUTION_Y) //Source Rect
	);
}
