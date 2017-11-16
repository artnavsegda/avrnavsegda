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

int main(void)
{
	unsigned int result;
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

	while(1);

	return 0;
}
