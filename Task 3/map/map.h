#ifndef MAP_H
#define MAP_H

typedef unsigned int uint32_t;
// Bitmap dimensions

#define BACKGROUND_IMAGE_WIDTH 608
#define BACKGROUND_IMAGE_HEIGHT 457
#define PIPE_WIDTH 80
#define GAP_HEIGHT 150
#define GROUND_HEIGHT 40   // adjust to match your ground image

// Pipe structure definition
typedef struct {
    int x;       // x position of pipe
    int gap_y;   // y position of top of the gap
} Pipe;

// Declarations of individual bitmap data arrays

extern const uint32_t game_bg[];
void drawPipe(Pipe *p, uint32_t color);



#endif 