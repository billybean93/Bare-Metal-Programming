#include "bird.h"
#include "../kernel/framebf.h"
#include "../map/map.h"

typedef enum { false = 0, true = 1 } bool;

void initBird(Bird *bird, int x, int y) {
    bird->x = x;
    bird->y = y;
    bird->vy = 0;
}

void updateBird(Bird *bird, char input, double dt) {
    bird->vy += (int)(800 * dt);
    if (bird->vy > 600) bird->vy = 600;

    bird->y += (int)(bird->vy * dt);

    int floorY = BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT - BIRD_HEIGHT;
    if (bird->y > floorY) {
        bird->y = floorY;
        bird->vy = 0;
    }
    if (bird->y < 0) {
        bird->y = 0;
        bird->vy = 0;
    }
}

void drawBird(Bird *bird, int bufferIndex, int frameCounter) {
    const unsigned int *sprite = ((frameCounter / 10) % 2 == 0) ? bird_up : bird_down;
    int angle_idx;
    if (bird->vy < -200) angle_idx = ANGLE_UP;
    else if (bird->vy < -50) angle_idx = ANGLE_SLIGHT;
    else if (bird->vy < 200) angle_idx = ANGLE_FLAT;
    else if (bird->vy < 500) angle_idx = ANGLE_DOWN;
    else angle_idx = ANGLE_SHARP;

    if (bufferIndex == 1)
        drawARGBSpriteRot(bird->x, bird->y, BIRD_WIDTH, BIRD_HEIGHT, sprite, 1, angle_idx);
    else
        drawARGBSpriteRot(bird->x, bird->y + BACKGROUND_IMAGE_HEIGHT, BIRD_WIDTH, BIRD_HEIGHT, sprite, 1, angle_idx);
}

// check collision
bool checkBirdPipeCollision(Bird *bird, Pipe *pipe) {
    int birdLeft   = bird->x;
    int birdRight  = bird->x + BIRD_WIDTH;
    int birdTop    = bird->y;
    int birdBottom = bird->y + BIRD_HEIGHT;

    int pipeLeft   = pipe->x;
    int pipeRight  = pipe->x + PIPE_WIDTH;
    int gapTop     = pipe->gap_y;
    int gapBottom  = pipe->gap_y + GAP_HEIGHT;

    if (birdRight > pipeLeft && birdLeft < pipeRight) {
        if (birdTop < gapTop) return true;      // bottom pipe
        if (birdBottom > gapBottom) return true; // top pipe
    }
    return false;
}
