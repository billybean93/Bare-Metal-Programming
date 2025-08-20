#ifndef BITMAP_H
#define BITMAP_H

typedef unsigned int uint32_t;
// Bitmap dimensions
#define CAT_DANCE_WIDTH 360
#define CAT_DANCE_HEIGHT 640

// Declarations of individual bitmap data arrays
extern const uint32_t cat_dance1[];
extern const uint32_t cat_dance2[];
extern const uint32_t cat_dance3[];
extern const uint32_t cat_dance4[];
extern const uint32_t cat_dance5[];
extern const uint32_t cat_dance6[];
extern const uint32_t cat_dance7[];
extern const uint32_t cat_dance8[];
extern const uint32_t cat_dance9[];
extern const uint32_t cat_dance10[];

// Declaration of the array of pointers to the bitmaps
extern const uint32_t *frames_order[];
extern const int NUM_FRAMES;

// The macro to get the number of frames

#endif // CAT_DANCE_VIDEO_H