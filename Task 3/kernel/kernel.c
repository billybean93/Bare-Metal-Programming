// #include "../uart/uart0.h"
// #include "../uart/uart1.h"
// #include "mbox.h"
// #include "framebf.h"
// #include "../map/map.h"
// #define WAIT_MSEC 50
// #define NUM_PIPES 5

// void handleInput(){}
// void updateGame(){}
// void main()
// {
//     // set up serial console
// 	uart_init();

// 	// say hello
// 	uart_puts("\n\nHello World!\n");

// 	// Initialize frame buffer
// 	framebf_init();
// 	Pipe pipe1 = { 400, 70 };  // initialize one pipe to test
// 	// Pipe data can't display below y = 70.



// 	Pipe pipes[NUM_PIPES];
// 	initPipes(pipes, NUM_PIPES, GROUND_HEIGHT ,BACKGROUND_IMAGE_HEIGHT );
// 	int bufferIndex = 1; // Start with double buffering
// 	int level = 1;
// 	int pipesFinished = 0;
// 	int playing = 1;
	
// while (playing) {
//     switch (level) {
//     case 1:
//         // Show start screen for Level 1
//         while (!drawGameStart(&bufferIndex, uart_read())) {
//             // drawGameStart returns 1 when SPACE is pressed
//         }

//         // Run level 1
//         while (pipes[NUM_PIPES - 1].x >= -(PIPE_WIDTH + 50)) {
//             updateMap(NUM_PIPES, pipes, &bufferIndex, WAIT_MSEC);
//         }

//         // Finished level
//         drawBackground(bufferIndex);
//         drawFinishedLv(bufferIndex);
// 		swapBuffer(bufferIndex);
// 		bufferIndex = !bufferIndex;	

//         level = 2;
//         initPipes(pipes, NUM_PIPES, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);
// 		wait_msec(1000);
//         break;

//     case 2:
// 		changeMapColor();
//         // Show start screen for Level 2
//         while (!drawGameStart(&bufferIndex, uart_read())) {}

//         // Run level 2
//         while (pipes[NUM_PIPES - 1].x >= -(PIPE_WIDTH + 50)) {
//             updateMap(NUM_PIPES, pipes, &bufferIndex, WAIT_MSEC);
//         }

//         drawBackground(bufferIndex);
//         drawFinishedLv(bufferIndex);
// 		swapBuffer(bufferIndex);
// 		bufferIndex = !bufferIndex;	

//         level = 3;
//         initPipes(pipes, NUM_PIPES, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);
// 		wait_msec(10000);
//         break;

//     case 3:
// 		changeMapColor();
//         // Show start screen for Level 3
//         while (!drawGameStart(&bufferIndex, uart_read())) {}

//         while (pipes[NUM_PIPES - 1].x >= -(PIPE_WIDTH + 50)) {
//             updateMap(NUM_PIPES, pipes, &bufferIndex, WAIT_MSEC);
//         }

//         drawBackground(bufferIndex);
//         drawFinishedLv(bufferIndex);

// 		swapBuffer(bufferIndex);
// 		bufferIndex = !bufferIndex;	

//         level = 1;  // loop back
//         initPipes(pipes, NUM_PIPES, GROUND_HEIGHT, BACKGROUND_IMAGE_HEIGHT);

//         break;

// 	default:
// 		playing = 0;
// 		break;
//     }

// }
	
	
// }

#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../map/map.h"
#include "../bird/bird.h"
#define WAIT_MSEC 16 // 60 fps
#define NUM_PIPES 12
void main() {
    framebf_init();
	uart_init();

	Pipe pipes[NUM_PIPES];
	Bird bird;
	initPipes(pipes, NUM_PIPES, GROUND_HEIGHT ,BACKGROUND_IMAGE_HEIGHT );
	initBird(&bird, 200, 200 );

	int bufferIndex = 0; // Start with double buffering
	int level = 1;
	int playing = 1;
	int frameCounter = 0;

	unsigned long long last_tick = cntpct_el0();
	double freq = (double)cntfrq_el0();


while (1) {
	unsigned long long now = cntpct_el0();
    double dt = (double)(now - last_tick) / freq;  // in seconds
    last_tick = now;
    frameCounter++;
    char c = uart_read();

    // Draw background and map
    drawMap(pipes, NUM_PIPES, bufferIndex);
	drawBird(bird.x, bird.y, bufferIndex, frameCounter);


    // Update & draw bird
    updateBird(&bird, c, dt);
	updatePipes(pipes, NUM_PIPES);

    swapBuffer(bufferIndex);
    bufferIndex = !bufferIndex;
    wait_msec(WAIT_MSEC);
}
}
