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

void SPI_Write(char c)
{
	for (int i = 0;i < 8; ++i)
	{
		PORTB |= _BV(PORTB7);
		if (bit_is_set(c,i-8))
			PORTB |= _BV(PORTB5);//set
		else
			PORTB &= ~(_BV(PORTB5));//clear
		PORTB &= ~(_BV(PORTB7));
	}
}

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
	PORTB &= ~(_BV(PORTB4));//clear
	for (int i = 0; i < NoBytes; i++)
		SPI_Write(buffer[i]);
	PORTB |= _BV(PORTB4);//set
}

char SPI_Read(void)
{
	char value = 0;
	for (int i = 0;i < 24; ++i)
	{
		PORTB |= _BV(PORTB7);
		if (bit_is_set(PINB,PINB6))
			value |= _BV(8-i);
		PORTB &= ~(_BV(PORTB7));
	}
	return value;
}

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
	PORTB &= ~(_BV(PORTB4));//clear
	for (int i = 0; i < NoBytes; i++)
		buffer[i] = SPI_Read();
	PORTB |= _BV(PORTB4);//set
}

void ad7705_reset()
{
	for (int i = 0;i < 40;++i)
	{
		PORTB |= _BV(PORTB7);
		PORTB &= ~(_BV(PORTB7));
	}
}

int main(void)
{
	uint16_t result;

	DDRB = _BV(PORTB5)|_BV(PORTB7);//mosi,sck
	DDRD = _BV(PORTD1)|_BV(PORTD5);//tx,clk
	startserial();
	starttimer();

	ad7705_reset();
	_delay_ms(10);
	SPI_Write_Bytes("\x20\x0C\x10\x04", 4);
	_delay_ms(10);
	SPI_Write_Bytes("\x60\x18\x3A\x00", 4);
	_delay_ms(10);
	SPI_Write_Bytes("\x70\x89\x78\xD7", 4);
	_delay_ms(10);

    while(1)
    {
			if (bit_is_clear(PINB,PINB4))
			{
				SPI_Write(0x38);
				SPI_Read_Bytes((char *)&result,2);
				printf("%04X\r\n",__builtin_bswap16(result));
			}
    }
	return 0;
}

