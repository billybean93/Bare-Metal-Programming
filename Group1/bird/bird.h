#ifndef BIRD_H
#define BIRD_H

#define BIRD_WIDTH   34
#define BIRD_HEIGHT  24

#define ANGLE_UP      0   // -25°
#define ANGLE_SLIGHT  1   // -10°
#define ANGLE_FLAT    2   //   0°
#define ANGLE_DOWN    3   // +30°
#define ANGLE_SHARP   4   // +70°

typedef struct {
    int x, y;
    int vy;   // vertical velocity
} Bird;

// 32-bit ARGB data sprite
extern const unsigned int bird_up[];
extern const unsigned int bird_mid[];
extern const unsigned int bird_down[];


extern const unsigned int BIRD_PALETTE[6];

void initBird(Bird *bird, int x, int y);
void updateBird(Bird *bird, char input, double dt);
void drawBird(Bird *bird, int bufferIndex, int frameCounter);

#endif
