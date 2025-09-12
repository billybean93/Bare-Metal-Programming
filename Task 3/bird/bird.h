#ifndef BIRD_H
#define BIRD_H

#define BIRD_WIDTH 34
#define BIRD_HEIGHT 24

// Bird struct
typedef struct {
    int x;          // position X (top-left)
    int y;          // position Y (top-left)
    int scale;      // scaling factor
    int alive;      // 1 = alive, 0 = dead (optional)
} Bird;

// Palette + sprites (declared elsewhere)
extern const unsigned int BIRD_PALETTE[6];
extern const unsigned int bird_up[34*24];
extern const unsigned int bird_mid[34*24];
extern const unsigned int bird_down[32*24];

// Functions
void drawBird(int x, int y, int bufferIndex, int fcount);

#endif