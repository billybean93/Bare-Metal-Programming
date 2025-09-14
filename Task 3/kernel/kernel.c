#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../map/map.h"
#include "../bird/bird.h"
#define WAIT_MSEC 16 // 60 fps
#define NUM_PIPES 2

void main()
{
    // set up serial console
	uart_init();

	// say hello
	uart_puts("\n\nHello World!\n");

	// Initialize frame buffer
	framebf_init();
	// Pipe pipe1 = { 400, 70 };  // initialize one pipe to test
	// Pipe data can't display below y = 70.

	Pipe pipes[NUM_PIPES];
	initPipes(pipes, NUM_PIPES, GROUND_HEIGHT ,BACKGROUND_IMAGE_HEIGHT );

	Bird bird;
	initBird(&bird, 200, 200 );

	int bufferIndex = 1; // Start with double buffering
	int level = 1;
	int playing = 1;

	int frameCounter = 0;
	char c = uart_read();


	unsigned long long last_tick = cntpct_el0();
	double freq = (double)cntfrq_el0();
	
	int started = 0;  // flag to indicate if we left start screen

	while (playing) {
		unsigned long long now = cntpct_el0();
		double dt = (double)(now - last_tick) / freq;
		last_tick = now;

		char c = uart_read();   // read once per frame
		frameCounter++;

		switch (level) {
			case 1:
				// --- Start Screen ---
				if (!started) {
					if (drawGameStart(&bufferIndex, c, frameCounter)) {
						started = 1;  // signal that we left start screen
					}
					break;  // stay in level 1, don’t run gameplay yet
				}

				// --- Gameplay loop ---
				if (pipes[NUM_PIPES - 1].x > -(PIPE_WIDTH)) {
					drawMap(pipes, NUM_PIPES, bufferIndex);
					drawBird(bird.x, bird.y, bufferIndex, frameCounter);

					updatePipes(pipes, NUM_PIPES);
					updateBird(&bird, c, dt);

					swapBuffer(bufferIndex);
					bufferIndex = !bufferIndex;
					wait_msec(WAIT_MSEC);
				} else {
					level = 2;
					started = 0;       // reset start flag for next level
					drawFinishedLv(bufferIndex);
					swapBuffer(bufferIndex);
					initPipes(pipes, NUM_PIPES, GROUND_HEIGHT ,BACKGROUND_IMAGE_HEIGHT ); // reinitialize pipes for new level
					wait_msec(2000);
				}
				break;

			case 2:
				if (!started) {
					if (drawGameStart(&bufferIndex, c, frameCounter)) {
						started = 1;
						initBird(&bird, 200, 200 ); //reset bird position for new level

					}
					break;
				}
				// --- Gameplay loop ---
				if (pipes[NUM_PIPES - 1].x > -(PIPE_WIDTH)) {
					drawMap(pipes, NUM_PIPES, bufferIndex);
					drawBird(bird.x, bird.y, bufferIndex, frameCounter);

					updatePipes(pipes, NUM_PIPES);
					updateBird(&bird, c, dt);

					swapBuffer(bufferIndex);
					bufferIndex = !bufferIndex;
					wait_msec(WAIT_MSEC);
				} else {
					level = 3;
					started = 0;       // reset start flag for next level
					drawFinishedLv(bufferIndex);
					swapBuffer(bufferIndex);
					initPipes(pipes, NUM_PIPES, GROUND_HEIGHT ,BACKGROUND_IMAGE_HEIGHT ); // reinitialize pipes for new level
					wait_msec(2000);
				}
				break;
			case 3:
			   if (!started) {
					if (drawGameStart(&bufferIndex, c, frameCounter)) {
						started = 1;
						initBird(&bird, 200, 200 );//reset bird position for new level

					}
					break;
				}
				// --- Gameplay loop ---
				if (pipes[NUM_PIPES - 1].x > -(PIPE_WIDTH)) {
					drawMap(pipes, NUM_PIPES, bufferIndex);
					drawBird(bird.x, bird.y, bufferIndex, frameCounter);

					updatePipes(pipes, NUM_PIPES);
					updateBird(&bird, c, dt);

					swapBuffer(bufferIndex);
					bufferIndex = !bufferIndex;
					wait_msec(WAIT_MSEC);
				} else {
					level = 3;
					started = 0;       // reset start flag for next level
					drawFinishedLv(bufferIndex);
					swapBuffer(bufferIndex);
					wait_msec(2000);
				}
				break;
			default:
				playing = 0;  // quit
				break;
		}
	}
	
	
}

// #include "../uart/uart0.h"
// #include "../uart/uart1.h"
// #include "mbox.h"
// #include "framebf.h"
// #include "../map/map.h"
// #include "../bird/bird.h"
// #define WAIT_MSEC 16 // 60 fps
// #define NUM_PIPES 12
// void main() {
//     framebf_init();
// 	uart_init();

// 	Pipe pipes[NUM_PIPES];
// 	Bird bird;
// 	initPipes(pipes, NUM_PIPES, GROUND_HEIGHT ,BACKGROUND_IMAGE_HEIGHT );
// 	initBird(&bird, 200, 200 );

// 	int bufferIndex = 0; // Start with double buffering
// 	int level = 1;
// 	int playing = 1;
// 	int frameCounter = 0;

// 	unsigned long long last_tick = cntpct_el0();
// 	double freq = (double)cntfrq_el0();


// while (1) {
// 	unsigned long long now = cntpct_el0();
//     double dt = (double)(now - last_tick) / freq;  // in seconds
//     last_tick = now;
//     frameCounter++;
//     char c = uart_read();

//     // Draw background and map
//     drawMap(pipes, NUM_PIPES, bufferIndex);
// 	drawBird(bird.x, bird.y, bufferIndex, frameCounter);


//     // Update & draw bird
//     updateBird(&bird, c, dt);
// 	updatePipes(pipes, NUM_PIPES);

//     swapBuffer(bufferIndex);
//     bufferIndex = !bufferIndex;
//     wait_msec(WAIT_MSEC);
// }
// }
