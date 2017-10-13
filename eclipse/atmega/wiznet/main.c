#include <avr/io.h>
#include <stdio.h>
#include "wizchip_conf.h"
#include "serial.h"

static void  wizchip_select(void)
{
	PORTB &= ~_BV(2);
}

static void  wizchip_deselect(void)
{
	PORTB |= _BV(2);
}

static uint8_t wizchip_read()
{
	uint8_t rb;
	SPDR = 0xFF;
	loop_until_bit_is_set(SPSR, SPIF);
	rb = SPDR;
	return rb;
}

static void  wizchip_write(uint8_t wb)
{
	SPDR = wb;
	loop_until_bit_is_set(SPSR, SPIF);
}

static void wizchip_readburst(uint8_t* pBuf, uint16_t len)
{
	for (int i = 0; i < len; i++)
	{
		SPDR = 0xFF;
		loop_until_bit_is_set(SPSR, SPIF);
		pBuf[i] = SPDR;
	}
}

static void  wizchip_writeburst(uint8_t* pBuf, uint16_t len)
{
	for (int i = 0; i < len; i++)
	{
		SPDR = pBuf[i];
		loop_until_bit_is_set(SPSR, SPIF);
	}
}

void start_ethernet(void)
{
	wiz_NetInfo netinfo = { .mac 	= {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},	// Mac address
	                            .ip 	= {192, 168, 1, 150},					// IP address
	                            .sn 	= {255, 255, 0, 0},					// Subnet mask
	                            .gw 	= {192, 168, 0, 1},						// Gateway address
	                            .dns 	= {8, 8, 8, 8},							// DNS server
	                            .dhcp 	= NETINFO_STATIC };						// DHCP enable / disable

	uint8_t memsize[2][8] = { { 2, 2, 2, 2 }, { 2, 2, 2, 2 } };
	reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
	reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
	reg_wizchip_spiburst_cbfunc(wizchip_readburst, wizchip_writeburst);
	ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize);
	ctlnetwork(CN_SET_NETINFO, (void*) &netinfo);
}

int main(void)
{
	start_serial();
	printf("Serial started\r\n");

	DDRB |= _BV(PORTB3)|_BV(PORTB5)|_BV(PORTB2);
	SPCR |= _BV(SPE)|_BV(MSTR);
	SPSR |= _BV(SPI2X);

	start_ethernet();
	printf("Ethernet started\r\n");

	while(1);

	return 0;
}
