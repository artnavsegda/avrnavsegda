#include <avr/io.h>
#include <util/delay.h>
#include "avr_compiler.h"
#include "spi_driver.h"

SPI_Master_t spiMasterC;

void spi_array(char *buffer, unsigned NoBytes)
{
	SPI_DataPacket_t dataPacket = { .ssPort = &PORTC, .ssPinMask = PIN4_bm, .transmitData = buffer, .bytesToTransceive = NoBytes };
	SPI_MasterTransceivePacket(&spiMasterC, &dataPacket);
}

int main(void)
{
	unsigned int result;
	PORTC.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm | PIN0_bm;
	PORTB.DIRSET = PIN7_bm;
	PORTC.PIN4CTRL = PORT_OPC_WIREDANDPULL_gc;
	PORTE.DIRSET = PIN2_bm;
	PORTC.OUTSET = PIN0_bm | PIN4_bm;

	SPI_MasterInit(&spiMasterC, &SPIC, &PORTC, false, SPI_MODE_0_gc, SPI_INTLVL_OFF_gc, false, SPI_PRESCALER_DIV4_gc);
	SPI_MasterSSLow(&PORTC, PIN4_bm);
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
	SPI_MasterSSHigh(&PORTC, PIN4_bm);

	while(1)
	{
		if (bit_is_clear(PORTC.IN,PIN1_bp))
		{
			SPI_MasterSSLow(&PORTC, PIN4_bm);
			SPI_MasterTransceiveByte(&spiMasterC, 0x38);
			spi_array((char *)&result,2);
			//printf("%04X\r\n",__builtin_bswap16(result));
			SPI_MasterSSHigh(&PORTC, PIN4_bm);
		}
	}
	return 0;

}
