#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../map/map.h"
#define DELAY_COUNT 1000000
#define NUM_PIPES 10


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
	
	while (1) {
    // Draw into back buffer
		drawMap(pipes, NUM_PIPES, bufferIndex);

		// Update game logic
		updatePipes(pipes, NUM_PIPES);

		// Flip the buffer you just drew
		swapBuffer(bufferIndex);

		// Toggle buffer for next frame
		bufferIndex = !bufferIndex;

		delay(DELAY_COUNT);  // crude framerate control
	}

	
	
}

