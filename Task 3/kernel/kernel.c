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

	
	//DISPLAY BACKGROUND IMAGE
	drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT,  game_bg);

	// draw the pipe
	// drawPipe(&pipe1, 0xFF00FF00);  // green
	// drawPipe(pipe1);

	Pipe pipes[3];
	initPipes(pipes, 3);
	drawPipes(pipes, 3);

	

	// echo everything back
	while(1) {
		//read each char
		char c = uart_getc();

		//send back 
		uart_sendc(c);
	}
}

