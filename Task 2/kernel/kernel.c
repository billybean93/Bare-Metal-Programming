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
	
	// display_video(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, frames_order, NUM_FRAMES, DELAY_COUNT);

	//DISPLAY BACKGROUND IMAGE AND MEMBER INFO
	drawBitmap(0, 0, BACKGROUND_IMAGE_WIDTH, BACKGROUND_IMAGE_HEIGHT,  background_img);
	drawString(15, 680, "ANH NGUYEN", 0xFFFFFF, 2);
	drawString(15, 700, "S3915181", 0xFFFFFF, 2);

	drawString(200, 680, "TRONG TRUONG", 0xFFFFFF, 2);
	drawString(200, 700, "S3872952", 0xFFFFFF, 2);
	
	drawString(426, 680, "TRIET TRAN", 0xFFFFFF, 2);
	drawString(426, 700, "S3979253", 0xFFFFFF, 2);

	drawString(635, 680, "LAM NGUYEN", 0xFFFFFF, 2);
	drawString(635, 700, "S3990403", 0xFFFFFF, 2);

	drawString(855, 680, "NAM DINH", 0xFFFFFF, 2);
	drawString(855, 700, "S3914547", 0xFFFFFF, 2);

	// echo everything back
	while(1) {
		//read each char
		char c = uart_getc();

		//send back 
		uart_sendc(c);
	}
}

