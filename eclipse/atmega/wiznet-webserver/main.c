#include <avr/io.h>
#include <stdio.h>
#include "wizchip_conf.h"
#include "serial.h"
#include "httpServer.h"
#include "webpage.h"

uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len)
{
	return 0;
}

uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len)
{
	return 0;
}

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

	uint8_t memsize[2][4] = { { 2, 2, 2, 2 }, { 2, 2, 2, 2 } };
	reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
	reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
	reg_wizchip_spiburst_cbfunc(wizchip_readburst, wizchip_writeburst);
	ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize);
	ctlnetwork(CN_SET_NETINFO, (void*) &netinfo);
}

void Display_Net_Conf()
{
	wiz_NetInfo gWIZNETINFO;
	ctlnetwork(CN_GET_NETINFO, (void*) &gWIZNETINFO);
	printf("\r\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	printf("IP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	printf("GW: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	printf("SN: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
}

uint8_t RX_BUF[1024];
uint8_t TX_BUF[1024];
uint8_t socknumlist[] = {0, 1, 2, 3};

int main(void)
{
	uint8_t tmpstr[6] = {0,};
	start_serial();
	printf("Serial started\r\n");

	DDRB |= _BV(PORTB3)|_BV(PORTB5)|_BV(PORTB2);
	SPCR |= _BV(SPE)|_BV(MSTR);
	SPSR |= _BV(SPI2X);

	start_ethernet();
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	printf("Wiznet %s started\r\n", tmpstr);
	Display_Net_Conf();

	httpServer_init(TX_BUF, RX_BUF, 4, socknumlist);

	reg_httpServer_webContent((uint8_t *)"index.html", (uint8_t *)index_page);

	display_reg_webContent_list();

	while(1)
	{
		for(int i = 0; i < 4; i++)
			httpServer_run(i);
	}

	return 0;
}
