#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

int main(void)
{
	UBRR0L = (F_CPU / (16UL * 9600)) - 1;
	UCSR0B = _BV(TXEN0) | _BV(RXEN0);

	stdout = &mystdout;

	printf("Hello eclipse World!\n\r");

    while(1)
    {
        //TODO:: Please write your application code
    }

	return 0;
}

