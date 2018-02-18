#include <avr/io.h>
#define F_CPU 1000000
#define BAUD 9600
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */

	MCUCR = 0x80; // XMEM enable
	SFIOR = 0x78; //bus keeper enable
	//SFIOR = 0x38; //bus keeper disable

	unsigned char * p = 0x500;
	unsigned char * d = 0x5FF;
	unsigned char x;

	stdout = &mystdout;
	_delay_ms(500);
	printf("hello\r\n");

	while (1)
	{
		*p = 0xFF;
		_delay_ms(1000);
		*d = 0xFF;
		_delay_ms(1000);

		*p = 0x00;
		_delay_ms(1000);
		*d = 0x00;
		_delay_ms(1000);

		*p = *d;
		_delay_ms(1000);
		*d = !*p;
		_delay_ms(1000);
	}
	return 0;
}

