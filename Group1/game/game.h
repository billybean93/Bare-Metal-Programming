#ifndef GAME_H
#define GAME_H

#include "../uart/uart0.h"
#include "../mailbox/mbox.h"
#include "../framebuffer/framebf.h"
#include "../map/map.h"
#include "../bird/bird.h"
#include "../timer/timer.h"

// ---------------------- Config ----------------------
#define WAIT_MSEC   16   // ~60 fps
#define NUM_PIPES   10

// ---------------------- API ----------------------

// Run the whole game
void game(void);

// Run one gameplay loop (returns 0 if game over, 1 if continue)
int runGameplayLoop(
    Pipe pipes[], int num_pipes,
    Bird *bird,
    int *bufferIndex,
    int *frameCounter,
    char c, double dt, int level
);

// UART helper to print signed integers
void uart_dec_signed(int num);


#endif // GAME_H