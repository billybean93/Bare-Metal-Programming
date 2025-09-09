#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../map/map.h"
#define WAIT_MSEC 50
#define NUM_PIPES 12

void handleInput(){}
void updateGame(){}
void main()
{
    // set up serial console
	uart_init();

	// say hello
	uart_puts("\n\nHello World!\n");

	// Initialize frame buffer
	framebf_init();
	Pipe pipe1 = { 400, 70 };  
	// Pipe data can't display below y = 70.



	Pipe pipes[NUM_PIPES];
	initPipes(pipes, NUM_PIPES, GROUND_HEIGHT ,BACKGROUND_IMAGE_HEIGHT );
	int bufferIndex = 1; // Start with double buffering
	// for (int i = 0; i < NUM_PIPES; i++) {
	// 	uart_dec(random(50));
		
	// 	uart_puts("\n");
	// 	delay(10000); // Small delay to get different random values
	// }
	// drawBackground(bufferIndex);
	// drawPipe(pipe1, bufferIndex);
   	
	// Game loop
	// while (pipes[NUM_PIPES - 1].x > 100) { // Continue until the last pipe goes off screen
    // // Draw into back buffer
	// 	drawMap(pipes, NUM_PIPES, bufferIndex);

	// 	// Update game logic
	// 	updatePipes(pipes, NUM_PIPES);

	// 	// Flip the buffer you just drew
	// 	swapBuffer(bufferIndex);

	// 	// Toggle buffer for next frame
	// 	bufferIndex = !bufferIndex;

	// 	wait_msec(WAIT_MSEC); // Control frame rate

	// }

	
	int level = 1;
	int finished = 0;
	int pipesFinished = 0;

	while (1) {
		switch (level)
		{
		case 1:
			while(pipes[NUM_PIPES-1].x >= -(PIPE_WIDTH + 100)){
	
				drawMap(pipes, NUM_PIPES, bufferIndex);

				// Draw into back buffer

				// Update game logic
				updatePipes(pipes, NUM_PIPES);

				// Flip the buffer you just drew
				swapBuffer(bufferIndex);

				// Toggle buffer for next frame
				bufferIndex = !bufferIndex;

				wait_msec(WAIT_MSEC); // Control frame rate
			}
			finished = 1;

			if (finished){

				level = 2;
				finished = 0;
			}
			else {

			}
			break;

		case 2:
			finished = 0;
			changeMapColor();
			if (finished){

				level = 3;
				finished = 0;
			}
			else {

			}
			break;

		case 3:

			if (finished){

				level = 1;
				finished = 0;
			}
			else {

			}
			break;
		
		default:
			break;
		}
		// do nothing. can't return from main()
	}

	
	
}

