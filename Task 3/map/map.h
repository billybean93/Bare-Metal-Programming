#ifndef MAP_H
#define MAP_H

typedef unsigned int uint32_t;
// Bitmap dimensions

#define BACKGROUND_IMAGE_WIDTH 1024
#define BACKGROUND_IMAGE_HEIGHT 770
#define PIPE_WIDTH 80
#define PIPE_SPEED 4
#define PIPE_HEIGHT 492
#define GAP_HEIGHT 150
#define GROUND_HEIGHT 62   // adjust to match your ground image
#define PIPE_SPACING 300   // horizontal distance between pipes
#define RAND_SEED 450
/* Define registers based on their memory addresses 
    (can do here or in gpio.h so that we can also use in other places) */
#define SYS_TIMER_CS  (* (volatile unsigned int*)(MMIO_BASE  + 0x00003000))
#define SYS_TIMER_CLO  (* (volatile unsigned int*)(MMIO_BASE + 0x00003004))
#define SYS_TIMER_CHI  (* (volatile unsigned int*)(MMIO_BASE + 0x00003008))


// Pipe structure definition
typedef struct {
    int x;       // x position of pipe
    int gap_y;   // y position of top of the gap
} Pipe;

// Declarations of individual bitmap data arrays

extern const uint32_t game_bg[];
extern const uint32_t pipe[];
void drawBackground(int noBuff);
void drawPipe(Pipe p, int noBuff);
void initPipes(Pipe pipes[], int num_pipes, unsigned int max, unsigned int min);
void drawPipes(Pipe pipes[], int num_pipes, int noBuff);
void updatePipes(Pipe pipes[], int num_pipes);
void drawMap(Pipe pipes[], int num_pipes, int noBuff);




#endif 