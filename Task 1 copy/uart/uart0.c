#include "./uart0.h"
#include "../mailbox/mbox.h"
/* PL011 reference: 48MHz UARTCLK assumed */
#define UARTCLK 48000000u

static void gpio_use_pl011(void)
{
	/* Set GPIO14,15 to ALT0 (TXD0,RXD0) */
	unsigned int r = GPFSEL1;
	r &= ~((7 << 12) | (7 << 15)); /* clear FSEL14, FSEL15 */
	r |= (4 << 12) | (4 << 15);	   /* set ALT0 */
	GPFSEL1 = r;

#ifdef RPI3
	/* Disable pulls then clock it for 14/15 (legacy method) */
	GPPUD = 0;
	delay(150);
	GPPUDCLK0 = (1 << 14) | (1 << 15);
	delay(150);
	GPPUDCLK0 = 0;
#else
	/* Pi4: set no pulls */
	unsigned int v = GPIO_PUP_PDN_CNTRL_REG0;
	/* Each gpio has 2 bits; 00 = none */
	v &= ~((3 << (14 * 2)) | (3 << (15 * 2)));
	GPIO_PUP_PDN_CNTRL_REG0 = v;
#endif
}

static void uart_disable(void)
{
	UART0_CR = 0;
}

static void uart_enable_txrx(void)
{
	UART0_CR = (1 << 0) /*UARTEN*/ | (1 << 8) /*TXE*/ | (1 << 9) /*RXE*/;
}

static void uart_config_8n1_fifo(void)
{
	UART0_LCRH = (1 << 4) /*FEN*/ | (3 << 5) /*WLEN=8bit*/;
}

int uart_set_baud(unsigned int baud)
{
	if (!(baud == 9600 || baud == 19200 || baud == 38400 || baud == 57600 || baud == 115200))
		return -1;
	unsigned int div = UARTCLK / (16 * baud);
	unsigned int rem = UARTCLK % (16 * baud);
	unsigned int fdiv = ((rem * 64) + (baud / 2)) / baud; /* rounded */

	UART0_IBRD = div;
	UART0_FBRD = fdiv;
	return 0;
}

void uart_set_flow(int enable)
{
	unsigned int cr = UART0_CR & ~((1 << 14) /*RTSEN*/ | (1 << 15) /*CTSEN*/);
	if (enable)
		cr |= (1 << 14) | (1 << 15);
	UART0_CR = cr;
}

void uart_init(void)
{
	uart_disable();
	gpio_use_pl011();

	/* Clear pending interrupts */
	UART0_ICR = 0x7FF;

	/* Default baud 115200, 8N1 FIFO on */
	uart_set_baud(115200);
	uart_config_8n1_fifo();

	/* Mask all interrupts */
	UART0_IMSC = 0;

	uart_enable_txrx();
}


void uart_sendc(char c)
{
	/* Wait until TX FIFO has space */
	while (UART0_FR & (1 << 5))
	{
	} /* TXFF */
	UART0_DR = (unsigned int)c;
}

char uart_getc(void)
{
	/* Wait until RX FIFO not empty */
	while (UART0_FR & (1 << 4))
	{
	} /* RXFE */
	char c = (char)(UART0_DR & 0xFF);
	return (c == '\r') ? '\n' : c;
}

void uart_puts(const char *s)
{
	while (*s)
	{
		if (*s == '\n')
			uart_sendc('\r');
		uart_sendc(*s++);
	}
}

/**
* Display a value in hexadecimal format
*/
void uart_hex(unsigned int num) {
	uart_puts("0x");
	for (int pos = 28; pos >= 0; pos = pos - 4) {

		// Get highest 4-bit nibble
		char digit = (num >> pos) & 0xF;

		/* Convert to ASCII code */
		// 0-9 => '0'-'9', 10-15 => 'A'-'F'
		digit += (digit > 9) ? (-10 + 'A') : '0';
		uart_sendc(digit);
	}
}

/*
**
* Display a value in decimal format
*/
void uart_dec(int num)
{
	//A string to store the digit characters
	char str[33] = {0};

	//Calculate the number of digits
	int len = 1;
	int temp = num;
	while (temp >= 10){
		len++;
		temp = temp / 10;
	}

	//Store into the string and print out
	for (int i = 0; i < len; i++){
		int digit = num % 10; //get last digit
		num = num / 10; //remove last digit from the number
		str[len - (i + 1)] = digit + '0';
	}
	str[len] = '\0';

	uart_puts(str);
}