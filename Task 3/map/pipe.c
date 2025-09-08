#include "map.h"
#include "../kernel/framebf.h"
#include "../uart/uart1.h"

unsigned int random(unsigned int N) {
    register unsigned long t;
   
    // Read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
 
    return 70 + (t % (N + 1)) ; //70 - N
}

void drawPipe(Pipe p, int bufferIndex) {

    int bottom_pipe_height = BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT - (p.gap_y + GAP_HEIGHT);

    if (bufferIndex) {
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

void initPipes(Pipe pipes[], int num_pipes, unsigned int max, unsigned int min) {
    for (int i = 0; i < num_pipes; i++) {

        pipes[i].x = 5 + (i+1) * PIPE_SPACING; // Start at x=400 and space out

        unsigned int gap_top = random(RAND_SEED); // Seed the random number generator

        pipes[i].gap_y = gap_top; // Vary gap_y for demonstration
        wait_msec(10); // Small delay to get different random values
        
    }
}

void drawPipes(Pipe pipes[], int num_pipes, int bufferIndex) {
    for (int i = 0; i < num_pipes; i++) {
        drawPipe(pipes[i], bufferIndex);
    }
}


void updatePipes(Pipe pipes[], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        pipes[i].x -= PIPE_SPEED;
    }
}

void drawBackground(int bufferIndex) {
    if (bufferIndex) {
        // If no double buffering, just draw directly
        drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
        return;
    }
    else {
        drawBitmap(0, BACKGROUND_IMAGE_HEIGHT, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
    }
    

}

void drawMap(Pipe pipes[], int num_pipes, int bufferIndex) {
    if (bufferIndex) {
        drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
        drawPipes(pipes, num_pipes, bufferIndex);
        return;
    }
    else {
        drawBitmap(0, BACKGROUND_IMAGE_HEIGHT, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
        drawPipes(pipes, num_pipes, bufferIndex);
    }
}


   

