#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../bitmap/bitmap.h"

#define DELAY_COUNT 100000000


void main()
{
    // set up serial console
	uart_init();

	// say hello
	uart_puts("\n\nHello World!\n");

	// Initialize frame buffer
	framebf_init();
	
	// /display_video(0, 0, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, frames_order, NUM_FRAMES, DELAY_COUNT);

	//DISPLAY BACKGROUND IMAGE AND MEMBER INFO
	

	drawBgAndNames();

	// echo everything back
	while(1) {
		//read each char
		char c = uart_getc();

		//send back 
		uart_sendc(c);
	}
}

