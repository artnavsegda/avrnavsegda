#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(USARTC0.STATUS,USART_DREIF_bp);
	USARTC0.DATA = c;
	return 0;
}

void startserial(void)
{
	PORTC.DIRSET = PIN3_bm;
	PORTC.DIRCLR = PIN2_bm;

	USARTC0.CTRLC = (uint8_t) USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | false;
	USARTC0.BAUDCTRLA = 12;

	USARTC0.CTRLB |= USART_RXEN_bm;
	USARTC0.CTRLB |= USART_TXEN_bm;

	stdout = &mystdout;

	printf("serial started\n\r");
}

void startspi(void)
{
	PORTC.DIRSET = PIN7_bm;
	PORTC.DIRSET = PIN5_bm;
	PORTC.DIRSET = PIN4_bm;//always activate chip select output when enabling spi
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm;

	printf("spi started\n\r");
}

char spi_transfer(char c)
{
	SPIC.DATA = c;
	loop_until_bit_is_set(SPIC.STATUS,SPI_IF_bp);
	return SPIC.DATA;
}

void spi_array(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
        {
				SPIC.DATA = buffer[i];
				loop_until_bit_is_set(SPIC.STATUS,SPI_IF_bp);
				buffer[i] = SPIC.DATA;
        }
}

int main(void)
{
	unsigned int result;
	startserial();
	startspi();
	PORTC.OUTCLR = PIN4_bm;
	spi_array("\xFF\xFF\xFF\xFF\xFF", 5);
	_delay_ms(10);
	spi_array("\x20\x0C", 2);
	_delay_ms(10);
	spi_array("\x10\x04", 2);
	_delay_ms(10);
	spi_array("\x60\x18\x3A\x00", 4);
	_delay_ms(10);
	spi_array("\x70\x89\x78\xD7", 4);
	_delay_ms(10);
	while(1)
	{
		if (bit_is_clear(PORTC.IN,PIN1_bp))
		{
			spi_transfer(0x38);
			spi_array((char *)&result,2);
			printf("%04X\r\n",BSWAP_16(result));
		}
	}
	return 0;
}

