#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

void startspi(void)
{
	PORTC.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm | PIN0_bm; //always activate chip select output when enabling spi
	PORTB.DIRSET = PIN7_bm; // SEN wiznet
	//PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
	PORTE.DIRSET = PIN2_bm;
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_1_gc; // idea of LSB first is completely wrong
	PORTC.OUTSET = PIN0_bm | PIN4_bm;
	PORTE.OUTSET = PIN2_bm;
	//PORTB.OUTCLR = PIN7_bm; // disable wiznet spi

	printf("spi started\n\r");
}

char spi_transfer(char c)
{
	SPIC.DATA = c;
	loop_until_bit_is_set(SPIC.STATUS,SPI_IF_bp);
	return SPIC.DATA;
}

void ads1256config()
{
	while(bit_is_set(PORTC.IN,PIN1_bp));
	PORTC.OUTCLR = PIN4_bm;
	_delay_us(2);
	spi_transfer(0x50|0);
	_delay_us(2);
	spi_transfer(0x03);
	_delay_us(2);
	spi_transfer(0x00);
	_delay_us(2);
	spi_transfer(0x08);
	_delay_us(2);
	spi_transfer(0x00);
	_delay_us(2);
	spi_transfer(0x03);
	PORTC.OUTSET = PIN4_bm;
	_delay_us(50);
}

int main(void)
{
	uint8_t result[3];
	_delay_ms(1000);
	startserial();
	startspi();

	while(bit_is_set(PORTC.IN,PIN1_bp));
	PORTC.OUTCLR = PIN4_bm;
	_delay_us(2);
	spi_transfer(0x10|0);
	_delay_us(2);
	spi_transfer(0x00);
	_delay_us(10);
	uint8_t _data = spi_transfer(0xFF);
	PORTC.OUTSET = PIN4_bm;
	printf("chip id %d\n",_data >> 4);

	ads1256config();

	while(1)
	{
		if (bit_is_clear(PORTC.IN,PIN1_bp))
		{
			PORTC.OUTCLR = PIN4_bm;
			_delay_us(2);
			spi_transfer(0x01);
			_delay_us(10);
			result[0] = spi_transfer(0xFF);
			result[1] = spi_transfer(0xFF);
			result[2] = spi_transfer(0xFF);
			PORTC.OUTSET = PIN4_bm;
			printf("%02X%02X%02X\r\n",result[0],result[1],result[2]);
		}
	}

	return 0;
}
