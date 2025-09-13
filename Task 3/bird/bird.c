#include "bird.h"
#include "../kernel/framebf.h"
#include "../map/map.h"
#include "../uart/uart1.h"

void initBird(Bird *bird,int x, int y){
    bird->x = x;
    bird->y = y;
    bird->vy = 0;
}


void updateBird(Bird *bird, char input, double dt) {
    // If space pressed -> jump (reset velocity upwards)
    if (input == ' ') {
        bird->vy = JUMP_VELOCITY;
    }

    // Apply gravity
    bird->vy += GRAVITY * dt;

    // Limit maximum fall speed
    if (bird->vy > MAX_FALL) {
        bird->vy = MAX_FALL; 
    }

    // Update position
    bird->y += bird->vy * dt;

    // Clamp to screen bounds
    if (bird->y < 0) {
        bird->y = 0;
    }
    if (bird->y > BACKGROUND_IMAGE_HEIGHT - BIRD_HEIGHT) {
        bird->y = BACKGROUND_IMAGE_HEIGHT - BIRD_HEIGHT;
    }
}

void drawBird(int x, int y, int bufferIndex, int frameCounter) {
    // Alternate every 10 frames
    const unsigned char *sprite;
    if ((frameCounter / 10) % 2 == 0) {
        sprite = bird_up;
    } else {
        sprite = bird_down;  // use your down-wing sprite here
    }

    if (bufferIndex == 1) {
        drawBitmap(x, y, BIRD_WIDTH, BIRD_HEIGHT, sprite);
    } else {
        drawBitmap(x, y + BACKGROUND_IMAGE_HEIGHT, BIRD_WIDTH, BIRD_HEIGHT, sprite);
    }
}

unsigned long cntfrq_el0() {
    unsigned long r;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(r));
    return r;
}

unsigned long long cntpct_el0() {
    unsigned long long r;
    asm volatile("mrs %0, cntpct_el0" : "=r"(r));
    return r;
}