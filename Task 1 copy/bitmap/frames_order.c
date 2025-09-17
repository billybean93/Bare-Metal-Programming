#include "bitmap.h"
typedef unsigned int uint32_t;

// Definition of the array of pointers
// This is the correct way to initialize the array with the bitmap pointers
const uint32_t *frames_order[] = {
    cat_dance1,
    cat_dance2,
    cat_dance3,
    cat_dance4,
    cat_dance5,
    cat_dance6,
    cat_dance7,
    cat_dance8,
    cat_dance9,
    cat_dance10
};

const int NUM_FRAMES = sizeof(frames_order) / sizeof(frames_order[0]);


