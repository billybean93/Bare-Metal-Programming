#include "./uart0.h"

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
