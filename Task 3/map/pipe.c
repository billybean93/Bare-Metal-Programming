#include "map.h"
#include "../kernel/framebf.h"

void drawPipe(Pipe p) {
    int bottom_pipe_height = BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT - (p.gap_y + GAP_HEIGHT);
    // Draw the pipe images
   drawBitmap(p.x, p.gap_y + GAP_HEIGHT,PIPE_WIDTH, bottom_pipe_height, pipe); // BOTTOM pipe


   drawBitmapFlipped(p.x, 0, PIPE_WIDTH, p.gap_y, pipe); // TOP pipe (flipped)}

}

void initPipes(Pipe pipes[], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        pipes[i].x = 10 + (i+1) * PIPE_SPACING; // Start at x=400 and space out
        pipes[i].gap_y = GAP_HEIGHT + ((i+1) * 10); // Vary gap_y for demonstration
    }
}

void drawPipes(Pipe pipes[], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        drawPipe(pipes[i]);
    }
}


