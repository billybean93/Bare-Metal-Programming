#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../map/map.h"
#include "../bird/bird.h"
#include "timer.h"

#define WAIT_MSEC 16 //60fps
#define NUM_PIPES 10

// Prototype
static inline int collideBirdPipe(const Bird *bird, const Pipe *p);
static inline int collideGround(const Bird *bird);
void uart_dec_signed(int num);

int runGameplayLoop(
    Pipe pipes[], int num_pipes,
    Bird *bird,
    int *bufferIndex,
    int *frameCounter,
    char c, double dt, int level
);

// ---------------------- UART helper ----------------------
void uart_dec_signed(int num) {
    if (num < 0) {
        uart_sendc('-');
        num = -num;
    }
    uart_dec(num);
}

// ---------------------- Main Entry ----------------------
void main(void) {
    uart_init();
    uart_puts("\n\n[INFO] Game Started\n");
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

        // Start screen
        if (!started) {
            if (drawGameStart(&bufferIndex, c, frameCounter, level)) {
                started = 1;
                uart_puts("[INFO] Level ");
                uart_dec(level);
                uart_puts(" Started\n");
            }
            continue;
        }

        if (pipes[NUM_PIPES - 1].x > -(PIPE_WIDTH)) {
            if (!runGameplayLoop(
                    pipes, NUM_PIPES,
                    &bird,
                    &bufferIndex,
                    &frameCounter,
                    c, dt, level)) {
                started = 0;
                level = 1;
                uart_puts("[WARN] GAME OVER, Reset to Level 1\n");
                continue;
            }
        } else {
            level++;
            started = 0;

            uart_puts("[INFO] Level Finished, Next Level: ");
            uart_dec(level);
            uart_puts("\n");

            drawFinishedLv(bufferIndex);
            swapBuffer(bufferIndex);
            bufferIndex = !bufferIndex;

            initPipes(pipes, NUM_PIPES, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);
            wait_msec(2000);
            initBird(&bird, 200, 200);
        }
    }
}

// ---------------------- Gameplay Loop ----------------------
int runGameplayLoop(
    Pipe pipes[], int num_pipes,
    Bird *bird,
    int *bufferIndex,
    int *frameCounter,
    char c, double dt, int level
) {
    drawMap(pipes, num_pipes, *bufferIndex);
    drawBird(bird, *bufferIndex, *frameCounter);

    updatePipes(pipes, num_pipes, PIPE_SPEED + level);

    // Handle SPACE input
    if (c == ' ') {
        int oldVy = bird->vy;
        int oldY  = bird->y;

        if (bird->vy > -200) {
            bird->vy = -300;  // Jump
            uart_puts("[ACTION] Bird Jumped (Y=");
            uart_dec(oldY);
            uart_puts(" Vy=");
            uart_dec_signed(oldVy);
            uart_puts(")\n");
        } else {
            uart_puts("[ERROR] Jump Pressed but No Jump Happened!\n");
            uart_puts("  Bird Y = "); uart_dec(oldY);
            uart_puts(" Vy = "); uart_dec_signed(oldVy);
            uart_puts("\n");
        }
    }

    updateBird(bird, 0, dt);

    // Collision detection
    int hit = 0;
    for (int i = 0; i < num_pipes; i++) {
        if (collideBirdPipe(bird, &pipes[i])) { hit = 1; break; }
    }
    if (!hit && collideGround(bird)) hit = 1;

    if (hit) {
        drawString(180, *bufferIndex ? 300 : 300 + BACKGROUND_IMAGE_HEIGHT,
                   "GAME OVER T.T", 1, 6);
        swapBuffer(*bufferIndex);
        *bufferIndex = !(*bufferIndex);
        wait_msec(1200);

        uart_puts("[ERROR] Collision Detected\n");

        initBird(bird, 200, 200);
        initPipes(pipes, num_pipes, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);
        return 0;
    }

    swapBuffer(*bufferIndex);
    *bufferIndex = !*bufferIndex;
    wait_msec(WAIT_MSEC);
    return 1;
}

// ---------------------- Collision Check ----------------------
static inline int collideBirdPipe(const Bird *bird, const Pipe *p) {
    int bx1 = bird->x, by1 = bird->y;
    int bx2 = bird->x + BIRD_WIDTH, by2 = bird->y + BIRD_HEIGHT;
    int px1 = p->x, px2 = p->x + PIPE_WIDTH;
    int top_y2 = p->gap_y;
    int bot_y1 = p->gap_y + GAP_HEIGHT;

    if (bx2 > px1 && bx1 < px2) {
        if (by1 < top_y2) return 1;
        if (by2 > bot_y1) return 1;
    }
    return 0;
}

static inline int collideGround(const Bird *bird) {
    return bird->y >= (BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT - BIRD_HEIGHT);
}
