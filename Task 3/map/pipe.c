#include "map.h"
#include "../kernel/framebf.h"

void drawPipe(Pipe p, int noBuff) {

    int bottom_pipe_height = BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT - (p.gap_y + GAP_HEIGHT);

    if (noBuff) {
        // If no double buffering, just draw directly
        // Draw the pipe images
        drawBitmap(p.x, p.gap_y + GAP_HEIGHT,PIPE_WIDTH, bottom_pipe_height, pipe); // BOTTOM pipe


        drawBitmapFlipped(p.x, 0, PIPE_WIDTH, p.gap_y, pipe); // TOP pipe (flipped)}
    } 
    else {
 // Draw to the off-screen buffer first
        drawBitmap(p.x, BACKGROUND_IMAGE_HEIGHT + p.gap_y + GAP_HEIGHT, PIPE_WIDTH, bottom_pipe_height, pipe); // BOTTOM pipe
        drawBitmapFlipped(p.x, BACKGROUND_IMAGE_HEIGHT, PIPE_WIDTH, p.gap_y, pipe); // TOP pipe (flipped)
    }

   
}

void initPipes(Pipe pipes[], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        pipes[i].x = 5 + (i+1) * PIPE_SPACING; // Start at x=400 and space out
        pipes[i].gap_y = GAP_HEIGHT + ((i+1) * 10); // Vary gap_y for demonstration
    }
}

void drawPipes(Pipe pipes[], int num_pipes, int noBuff) {
    for (int i = 0; i < num_pipes; i++) {
        drawPipe(pipes[i], noBuff);
    }
}


void updatePipes(Pipe pipes[], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        pipes[i].x -= PIPE_SPEED;
    }
}

void drawBackground(int noBuff) {
    if (noBuff) {
        // If no double buffering, just draw directly
        drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
        return;
    }
    else {
        drawBitmap(0, BACKGROUND_IMAGE_HEIGHT, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
    }
    

}

void drawMap(Pipe pipes[], int num_pipes, int noBuff) {
    if (noBuff) {
        drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
        drawPipes(pipes, num_pipes, noBuff);
        return;
    }
    else {
        drawBitmap(0, BACKGROUND_IMAGE_HEIGHT, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
        drawPipes(pipes, num_pipes, noBuff);
    }
}
   

