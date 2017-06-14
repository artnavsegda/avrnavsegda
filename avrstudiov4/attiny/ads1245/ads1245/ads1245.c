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

void startserial(void)
{
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
}

void starttimer(void)
{
	TCCR0A = 0x33;
	TCCR0B = 0x09;
	OCR0A = 0x06;
	OCR0B = 0x03;
}

uint32_t shiftIn(void)
{
	uint32_t value = 0;
	uint8_t i;
	for (i = 0;i < 24; ++i)
	{
		PORTD |= _BV(PORTD4);
		if (bit_is_set(PIND,PIND3))
			value |= (long)1<<(24-i);
		PORTD &= ~(_BV(PORTD4));
	}
	PORTD |= _BV(PORTD4);
	PORTD &= ~(_BV(PORTD4));// 25th pulse to keep DATA high till next data ready
	return value;
}

int main(void)
{
	DDRD = _BV(PORTD1)|_BV(PORTD5)|_BV(PORTD4);//tx,clk,sck
	startserial();
	starttimer();

    while(1)
    {
			if (bit_is_clear(PIND,PIND3))
			{
				printf("%lX\r\n",shiftIn());
			}
    }
	return 0;
}

