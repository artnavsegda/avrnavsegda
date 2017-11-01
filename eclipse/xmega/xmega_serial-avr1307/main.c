#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "usart_driver.h"

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	USART_PutChar(&USARTC0, c);
	return 0;
}

int main(void)
{
	PORTC.DIRSET = PIN3_bm;
	PORTC.DIRCLR = PIN2_bm;
	USART_Format_Set(&USARTC0, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);
	USART_Baudrate_Set(&USARTC0, 12 , 0);
	USART_Rx_Enable(&USARTC0);
	USART_Tx_Enable(&USARTC0);

	stdout = &mystdout;
	printf("Hello eclipse World!\n\r");

	while(1)
	{
		_delay_ms(1000);
		printf("hello\n\r");
	}
}
