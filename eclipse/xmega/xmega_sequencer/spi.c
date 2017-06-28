#include <avr/io.h>
#include <stdio.h>

void startspi(void)
{
	PORTC.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm; //always activate chip select output when enabling spi
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
