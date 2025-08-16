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


	while (1){
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance1);
		delay(DELAY_COUNT); // Delay to see the first image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance2);
		delay(DELAY_COUNT); // Delay to see the second image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance3);
		delay(DELAY_COUNT); // Delay to see the third image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance4);
		delay(DELAY_COUNT); // Delay to see the fourth image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance5);
		delay(DELAY_COUNT); // Delay to see the fifth image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance6);
		delay(DELAY_COUNT); // Delay to see the sixth image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance7);
		delay(DELAY_COUNT); // Delay to see the seventh image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance8);
		delay(DELAY_COUNT); // Delay to see the eighth image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance9);
		delay(DELAY_COUNT); // Delay to see the ninth image
		drawBitmap(100, 50, CAT_DANCE_WIDTH, CAT_DANCE_HEIGHT, cat_dance10);
		delay(DELAY_COUNT); // Delay to see the tenth image
	}




	// echo everything back
	while(1) {
		//read each char
		char c = uart_getc();

		//send back 
		uart_sendc(c);
	}
}

