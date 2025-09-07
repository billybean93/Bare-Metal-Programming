#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../map/map.h"
#define DELAY_COUNT 100000000


void main()
{
    // set up serial console
	uart_init();

	// say hello
	uart_puts("\n\nHello World!\n");

	// Initialize frame buffer
	framebf_init();
	Pipe pipe1 = { 400, 100 };  // x=400px, gap starts at y=250


	Pipe pipes[3];
	initPipes(pipes, 3);
	int bufferIndex = 1; // Start with double buffering
	
	while (1) {
    // Draw into back buffer
		drawMap(pipes, 3, bufferIndex);

		// Update game logic
		updatePipes(pipes, 3);

		// Flip the buffer you just drew
		swapBuffer(bufferIndex);

		// Toggle buffer for next frame
		bufferIndex = !bufferIndex;

		delay(DELAY_COUNT);  // crude framerate control
	}
	
}

