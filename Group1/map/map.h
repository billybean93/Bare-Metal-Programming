#ifndef MAP_H
#define MAP_H

typedef unsigned int uint32_t;

#define BACKGROUND_IMAGE_WIDTH 1024
#define BACKGROUND_IMAGE_HEIGHT 770
#define PIPE_WIDTH 80
#define PIPE_SPEED 3
#define PIPE_HEIGHT 492
#define GAP_HEIGHT 150 
#define GROUND_HEIGHT 62
#define PIPE_SPACING 400
#define RAND_SEED 420

#define FINISH_LINE_WIDTH 60
#define STRIPE_HEIGHT 12
#define STRIPE_WIDTH 12

typedef struct {
    int x;
    int gap_y;
} Pipe;

extern const uint32_t game_bg[];
extern const uint32_t pipe[];
extern const uint32_t finish_line[];

void drawBackground(int bufferIndex);
void drawPipe(Pipe p, int bufferIndex);
void initPipes(Pipe pipes[], int num_pipes, unsigned int max, unsigned int min);
void drawPipes(Pipe pipes[], int num_pipes, int bufferIndex);
void updatePipes(Pipe pipes[], int num_pipes, int speed);
void drawMap(Pipe pipes[], int num_pipes, int bufferIndex);
void drawFinishLine(int x, int bufferIndex);
void updateMap(int num_pipes, Pipe pipes[], int *bufferIndex, unsigned int msVal, int fcount);

#endif
