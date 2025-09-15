#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../map/map.h"
#include "../bird/bird.h"

extern unsigned long long cntpct_el0(void);
extern unsigned int cntfrq_el0(void);
void wait_msec(unsigned int n);

#define WAIT_MSEC 16
#define NUM_PIPES 2

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

// ---------------------- Main Entry ----------------------
void main(void) {
    uart_init();
    uart_puts("\n\nHello World!\n");
    framebf_init();

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

    while (playing) {
        unsigned long long now = cntpct_el0();
        double dt = (double)(now - last_tick) / freq;
        last_tick = now;

        char c = uart_read();
        frameCounter++;

        // ---------------------- Start Screen ----------------------
        if (!started) {
            if (drawGameStart(&bufferIndex, c, frameCounter)) {
                started = 1;
            }
            continue; // skip rest of loop until started
        }

        // ---------------------- Gameplay Loop ----------------------
        if (pipes[NUM_PIPES - 1].x > -(PIPE_WIDTH)) {
            drawMap(pipes, NUM_PIPES, bufferIndex);
            drawBird(&bird, bufferIndex, frameCounter);

            updatePipes(pipes, NUM_PIPES);
            updateBird(&bird, c, dt);

            // ---- Collision check ----
            int hit = 0;
            for (int i = 0; i < NUM_PIPES; i++) {
                if (collideBirdPipe(&bird, &pipes[i])) { hit = 1; break; }
            }
            if (!hit && collideGround(&bird)) hit = 1;

            if (hit) {
                // GAME OVER
                drawString(180, bufferIndex ? 300 : 300 + BACKGROUND_IMAGE_HEIGHT, "GAME OVER", 1, 6);
                swapBuffer(bufferIndex);
                bufferIndex = !bufferIndex;
                wait_msec(1200);

                // reset
                started = 0;
                level = 1;  // Return to level 1
                initBird(&bird, 200, 200);
                initPipes(pipes, NUM_PIPES, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);
                continue;
            }

            // swap buffer to display the frame
            swapBuffer(bufferIndex);
            bufferIndex = !bufferIndex;
            wait_msec(WAIT_MSEC);

        } else {
            // ---- Level Finished ----
            level++;
            started = 0; // Return to the start screen for the new level

            drawFinishedLv(bufferIndex);
            swapBuffer(bufferIndex);
            bufferIndex = !bufferIndex;

            initPipes(pipes, NUM_PIPES, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);
            wait_msec(2000);

            initBird(&bird, 200, 200);
        }
    }
}
