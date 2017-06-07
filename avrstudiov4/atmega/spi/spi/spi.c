#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#define BAUD 9600
#include <util/setbaud.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

void startserial(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
	stdout = &mystdout;
	printf("Hello eclipse World!\n\r");
}

void startspi(void)
{
	DDRB = _BV(PORTB3)|_BV(PORTB5)|_BV(PORTB2);
	SPCR = _BV(SPE)|_BV(MSTR);
	SPSR |= _BV(SPI2X);
}

char spi_transfer(char c)
{
	PORTB &= ~(_BV(PORTB2));//clear
	SPDR = c;
	loop_until_bit_is_set(SPSR, SPIF);
	PORTB |= _BV(PORTB2);//set
	return(SPDR);
}

void SPI_Read_Bytes(char *buffer, unsigned NoBytes)
{
	PORTB &= ~(_BV(PORTB2));//clear
	for (int i = 0; i < NoBytes; i++)
	{
		SPDR = 0xFF;
		loop_until_bit_is_set(SPSR, SPIF);
		buffer[i] = SPDR;
	}
	PORTB |= _BV(PORTB2);//set
}

void SPI_Write_Bytes(char *buffer, unsigned NoBytes)
{
	PORTB &= ~(_BV(PORTB2));//clear
	for (int i = 0; i < NoBytes; i++)
	{
		SPDR = buffer[i];
		loop_until_bit_is_set(SPSR, SPIF);
	}
	PORTB |= _BV(PORTB2);//set
}

int main(void)
{
	uint16_t result;
	startserial();
	startspi();
	SPI_Write_Bytes("\xFF\xFF\xFF\xFF\xFF", 5);
	_delay_ms(10);
	SPI_Write_Bytes("\x20\x0C\x10\x04", 4);
	_delay_ms(10);
	SPI_Write_Bytes("\x60\x18\x3A\x00", 4);
	_delay_ms(10);
	SPI_Write_Bytes("\x70\x89\x78\xD7", 4);
	_delay_ms(10);

    while(1)
    {
			if (bit_is_clear(PINB,PINB1))
			{
				spi_transfer(0x38);
				SPI_Read_Bytes((char *)&result,2);
				printf("%04X\r\n",__builtin_bswap16(result));
			}
    }
	return 0;
}
