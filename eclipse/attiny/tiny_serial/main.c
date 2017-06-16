#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#define BAUD 9600
#include <util/setbaud.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
	return 0;
}

int main(void)
{
	int i = 0;
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;

#if USE_2X
    UCSRA |= _BV(U2X);
#else
    UCSRA &= ~(_BV(U2X));
#endif

    UCSRC = _BV(UCSZ1) | _BV(UCSZ0); /* 8-bit data */
    UCSRB = _BV(RXEN) | _BV(TXEN);   /* Enable RX and TX */

	stdout = &mystdout;

	printf("Hello eclipse World!\n\r");


    while(1)
    {
    		printf("count %d\n\r", i++);
    		_delay_ms(1000);
    }

	return 0;
}

