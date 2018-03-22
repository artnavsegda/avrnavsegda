#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#define BAUD 9600
#include <util/setbaud.h>

#include <avr/interrupt.h>

char buffer[100];
int pos = 0;

ISR(USART_RX_vect)
{
	pos = 0;
	while(1)
	{
		while(bit_is_set(UCSR0A,RXC0))
			buffer[pos++] = UDR0;
		_delay_ms(2);
		if (bit_is_clear(UCSR0A,RXC0))
			break;
	}
}

#include <stdio.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

int main()
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0); /* Enable RX and TX */

	stdout = &mystdout;

	sei();

	_delay_ms(100);

	printf("Hello World!\n\r");

	_delay_ms(100);

	while(1)
	{
		if (pos > 0)
		{
			printf("recieved %d data\r\n",pos);
			pos = 0;
		}	
		_delay_ms(100);
	}
}

