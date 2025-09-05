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
	
	//DISPLAY BACKGROUND IMAGE
	drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT,  game_bg);
	

	// echo everything back
	while(1) {
		//read each char
		char c = uart_getc();

		//send back 
		uart_sendc(c);
	}
}

