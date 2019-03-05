#pragma once

#define WINDOW_NAME		"SET Trek"
#define RESOLUTION_X	1027
#define RESOLUTION_Y	768
#define GRID_STARTING_X 100.0
#define GRID_STARTING_Y 10.0

#define SHIP_DELAY 500	// in milliseconds


// Find which resolution is smaller. Than make the grid 100 less than the smallest
#if RESOLUTION_X >= RESOLUTION_Y
#define GRID_SIZE (RESOLUTION_Y-100)
#else
#define GRID_SIZE (RESOLUTION_X-100)
#endif

// Find which resolution is smaller. Than make the grid 100 less than the smallest
#define PROP_SCALE (GRID_SIZE/1300.0)
#define PROP_CENTER (GRID_SIZE/35.0)
