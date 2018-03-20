#include <avr/io.h>
#define F_CPU 2000000
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

unsigned short adc(unsigned char controlbyte)
{
	volatile unsigned char *p = (unsigned char *) 0x500;
	volatile unsigned short *r = 0x500;
	unsigned short z;
	*p = controlbyte;
	loop_until_bit_is_clear(PIND, PD2);
	z = *r;
	return z;
}

int main(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif

	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
	UCSR0C = _BV(URSEL0) | _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */

	MCUCR = 0x80; // XMEM enable
	//SFIOR = 0x78; //bus keeper enable
	SFIOR = 0x38; //bus keeper disable

	stdout = &mystdout;
	_delay_ms(500);
	printf("hello\r\n");

	while (1)
	{
		printf("%x %x %x %x %x %x %x %x\r\n",adc(0), adc(1), adc(2), adc(3), adc(4), adc(5), adc(6), adc(7));
	}

	return 0;
}

