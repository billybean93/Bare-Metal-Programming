#include "map.h"
#include "../kernel/framebf.h"

void drawPipe(Pipe *p) {
    int bottom_pipe_height = BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT - (p->gap_y + GAP_HEIGHT);
    // Draw the pipe images
   drawBitmap(p->x, p->gap_y + GAP_HEIGHT,PIPE_WIDTH, bottom_pipe_height, pipe); // BOTTOM pipe


   drawBitmapFlipped(p->x, 0, PIPE_WIDTH, p->gap_y, pipe); // TOP pipe (flipped)}

}

