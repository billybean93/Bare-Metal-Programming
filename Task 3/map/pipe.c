#include "map.h"
#include "../kernel/framebf.h"
#include "../uart/uart1.h"
#include "../bird/bird.h"

unsigned int random(unsigned int N) {
    register unsigned long t;
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    return 70 + (t % (N + 1));
}

void drawPipe(Pipe p, int bufferIndex) {
    int bottom_pipe_height = BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT - (p.gap_y + GAP_HEIGHT);

    if (bufferIndex) {
        drawBitmap(p.x, p.gap_y + GAP_HEIGHT, PIPE_WIDTH, bottom_pipe_height, pipe);
        drawBitmapFlipped(p.x, 0, PIPE_WIDTH, p.gap_y, pipe);
    } else {
        drawBitmap(p.x, BACKGROUND_IMAGE_HEIGHT + p.gap_y + GAP_HEIGHT, PIPE_WIDTH, bottom_pipe_height, pipe);
        drawBitmapFlipped(p.x, BACKGROUND_IMAGE_HEIGHT, PIPE_WIDTH, p.gap_y, pipe);
    }
}

void initPipes(Pipe pipes[], int num_pipes, unsigned int max, unsigned int min) {
    for (int i = 0; i < num_pipes; i++) {
        pipes[i].x = BACKGROUND_IMAGE_WIDTH + PIPE_WIDTH + (i + 1) * PIPE_SPACING;
        unsigned int gap_top = random(RAND_SEED);
        pipes[i].gap_y = gap_top;
        wait_msec(10);
    }
}

void drawPipes(Pipe pipes[], int num_pipes, int bufferIndex) {
    for (int i = 0; i < num_pipes; i++) {
        if (pipes[i].x > -PIPE_WIDTH && pipes[i].x < BACKGROUND_IMAGE_WIDTH)
            drawPipe(pipes[i], bufferIndex);
    }
}

void updatePipes(Pipe pipes[], int num_pipes, int speed) {
    for (int i = 0; i < num_pipes; i++)
        pipes[i].x -= speed;
}

void drawBackground(int bufferIndex) {
    if (bufferIndex)
        drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
    else
        drawBitmap(0, BACKGROUND_IMAGE_HEIGHT, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
}

void drawMap(Pipe pipes[], int num_pipes, int bufferIndex) {
    if (bufferIndex) {
        drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
        drawPipes(pipes, num_pipes, bufferIndex);
    } else {
        drawBitmap(0, BACKGROUND_IMAGE_HEIGHT, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT, game_bg);
        drawPipes(pipes, num_pipes, bufferIndex);
    }
}
