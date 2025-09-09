#include "map.h"
#include "../kernel/framebf.h"
#include "../uart/uart1.h"


// Simple linear congruential generator for pseudo-random numbers

unsigned int random(unsigned int N) {
    register unsigned long t;
   
    // Read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
 
    return 70 + (t % (N + 1)) ; //70 - N
}

void drawFinishLine(int x, int bufferIndex) {
    int y = (bufferIndex == 1) ? 0 : BACKGROUND_IMAGE_HEIGHT;
    int maxHeight = BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT;

    // Loop down the screen
    while (y < maxHeight) {
        // Loop across columns inside the finish line width
        for (int col = 0; col < FINISH_LINE_WIDTH; col += STRIPE_WIDTH) {
            // Alternate colors: depends on row + col
            uint32_t color;
            if (((y / STRIPE_HEIGHT) + (col / STRIPE_WIDTH)) % 2 == 0) {
                color = 0xA7C7E7; // white
            } else {
                color = 0xFF000000; // black
            }

            // Draw one checker square
            drawRectARGB32(
                x + col,              // x1
                y,                    // y1
                x + col + STRIPE_WIDTH,
                y + STRIPE_HEIGHT,
                color,
                1 // filled
            );
        }

        y += STRIPE_HEIGHT; // move to next row
    }
}

// Function to draw a single pipe (top and bottom)

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
// Function to initialize pipes with random gap positions

void initPipes(Pipe pipes[], int num_pipes, unsigned int max, unsigned int min) {
    for (int i = 0; i < num_pipes; i++) {

        pipes[i].x = BACKGROUND_IMAGE_WIDTH + PIPE_WIDTH + (i+1) * PIPE_SPACING; // Start at x=400 and space out

        unsigned int gap_top = random(RAND_SEED); // Seed the random number generator

        pipes[i].gap_y = gap_top; // Vary gap_y for demonstration
        wait_msec(10); // Small delay to get different random values
        
    }

}
// Function to draw all pipes

void drawPipes(Pipe pipes[], int num_pipes, int bufferIndex) {
    for (int i = 0; i < num_pipes; i++) {
        if (pipes[i].x > -PIPE_WIDTH && pipes[i].x < BACKGROUND_IMAGE_WIDTH) {

            drawPipe(pipes[i], bufferIndex);
        }
    }
}



// Function to update pipe positions


void updatePipes(Pipe pipes[], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        pipes[i].x -= PIPE_SPEED;
    }
}
// Function to draw the background

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
// Function to draw the entire map (background + pipes)

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

// Function to draw the finish line with a checkerboard pattern




