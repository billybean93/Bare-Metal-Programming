#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../map/map.h"
#include "../bird/bird.h"


#define WAIT_MSEC 16 
#define NUM_PIPES 5

static inline int collideBirdPipe(const Bird *bird, const Pipe *p);
static inline int collideGround(const Bird *bird);
int runGameplayLoop(
	Pipe pipes[], int num_pipes,
	Bird *bird,
	int *bufferIndex,
	int *frameCounter,
	char c, double dt
);
int levelEnded(int num_pipes, Pipe pipes[]);

// ---------------------- Main Entry ----------------------
void main(void) {
    uart_init();
    uart_puts("\n\nHello World!\n");
    framebf_init();

	// ---------------------- Game Initialization ----------------------
    Pipe pipes[NUM_PIPES];
    Bird bird;

    int bufferIndex = 1;
    int level = 1;
    int playing = 1;
    int started = 0;
    int frameCounter = 0;

    initPipes(pipes, NUM_PIPES, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);
    initBird(&bird, 200, 200);

    unsigned long long last_tick = cntpct_el0();
    double freq = (double)cntfrq_el0();

	// ---------------------- Game Loop ----------------------

    while (playing) {
		// Calculate time elapsed since last frame
        unsigned long long now = cntpct_el0();
        double dt = (double)(now - last_tick) / freq;
        last_tick = now;
		// Read user input (non-blocking)
        char c = uart_read();
        frameCounter++; // for animation

        // ---------------------- Start Screen ----------------------
        if (!started) {
            if (drawGameStart(&bufferIndex, c, frameCounter, level)) {
                started = 1;
            }
            continue; // skip rest of loop until started
        }
		if (pipes[NUM_PIPES- 1].x > -(PIPE_WIDTH)) {
			if (!runGameplayLoop
				(
					pipes, NUM_PIPES, 
					&bird,
					&bufferIndex, 
					&frameCounter,
					c, 
					dt
				)
				) 

			{
				started = 0;
				level = 1; // reset to level 1 on game over
				continue;
			}
		} else {
			// ---- Level Finished ----
			level++;
			started = 0;
			drawFinishedLv(bufferIndex);
			swapBuffer(bufferIndex);
			bufferIndex = !bufferIndex;

			initPipes(pipes, NUM_PIPES, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);
			wait_msec(2000);

			initBird(&bird, 200, 200);
		}
	}
}


int runGameplayLoop(
    Pipe pipes[], int num_pipes,
    Bird *bird,
    int *bufferIndex,
    int *frameCounter,
    char c, double dt
) {
    // Draw map and bird
    drawMap(pipes, num_pipes, *bufferIndex);
    drawBird(bird, *bufferIndex, *frameCounter);

    updatePipes(pipes, num_pipes);
    updateBird(bird, c, dt);

    // ---- Collision check ----
    int hit = 0;
    for (int i = 0; i < num_pipes; i++) {
        if (collideBirdPipe(bird, &pipes[i])) { hit = 1; break; }
    }
    if (!hit && collideGround(bird)) hit = 1;

    if (hit) {
        // GAME OVER
        drawString(180, *bufferIndex ? 300 : 300 + BACKGROUND_IMAGE_HEIGHT,
                   "GAME OVER", 1, 6);
        swapBuffer(*bufferIndex);
        *bufferIndex = !(*bufferIndex);
        wait_msec(1200);

        // Reset state
        initBird(bird, 200, 200);
        initPipes(pipes, num_pipes, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);

        return 0; // signal "game over"
    }

    // Swap buffer to display the frame
    swapBuffer(*bufferIndex);
    *bufferIndex = !(*bufferIndex);
    wait_msec(WAIT_MSEC);

    return 1; // still playing
}

// ---------------------- Collision Check ----------------------
static inline int collideBirdPipe(const Bird *bird, const Pipe *p) {
    int bx1 = bird->x, by1 = bird->y;
    int bx2 = bird->x + BIRD_WIDTH, by2 = bird->y + BIRD_HEIGHT;
    int px1 = p->x, px2 = p->x + PIPE_WIDTH;
    int top_y2 = p->gap_y;
    int bot_y1 = p->gap_y + GAP_HEIGHT;

    if (bx2 > px1 && bx1 < px2) {
        if (by1 < top_y2) return 1;  // touch the upper tube
        if (by2 > bot_y1) return 1;  // touch the lower tube
    }
    return 0;
}

static inline int collideGround(const Bird *bird) {
    return bird->y >= (BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT - BIRD_HEIGHT);
}
