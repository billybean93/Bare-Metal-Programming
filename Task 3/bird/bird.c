#include "bird.h"
#include "../kernel/framebf.h"
#include "../map/map.h"



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