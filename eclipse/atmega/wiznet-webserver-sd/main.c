#include "wizchip_conf.h"
#include "httpServer.h"
#include "diskio.h"

#define DATA_BUF_SIZE   1024
uint8_t RX_BUF[DATA_BUF_SIZE];
uint8_t TX_BUF[DATA_BUF_SIZE];

#define MAX_HTTPSOCK	2
uint8_t socknumlist[] = {2, 3};

static void  wizchip_select(void)
{

}

static void  wizchip_deselect(void)
{

}

static uint8_t wizchip_read()
{
	uint8_t rb;
	return rb;
}

static void  wizchip_write(uint8_t wb)
{

}

static void wizchip_readburst(uint8_t* pBuf, uint16_t len)
{

}

static void  wizchip_writeburst(uint8_t* pBuf, uint16_t len)
{

}

uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1;        // ret = 1 means 'uri_name' matched
	return ret;
}

uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1;        // ret = '1' means 'uri_name' matched
	return ret;
}

void start_ethernet(void)
{
	wiz_NetInfo netinfo = { .mac 	= {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},	// Mac address
	                            .ip 	= {192, 168, 0, 112},					// IP address
	                            .sn 	= {255, 255, 255, 0},					// Subnet mask
	                            .gw 	= {192, 168, 0, 1},						// Gateway address
	                            .dns 	= {8, 8, 8, 8},							// DNS server
	                            .dhcp 	= NETINFO_STATIC };						// DHCP enable / disable

	uint8_t memsize[2][8] = { { 2, 2, 2, 2, 2, 2, 2, 2 }, { 2, 2, 2, 2, 2, 2, 2, 2 } };
	reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
	reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
	reg_wizchip_spiburst_cbfunc(wizchip_readburst, wizchip_writeburst);
	ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize);
	ctlnetwork(CN_SET_NETINFO, (void*) &netinfo);
}

void start_timer(void)
{
	//OCR0A = F_CPU / 1024 / 100 - 1;
	//TCCR0A = _BV(WGM01);
	//TCCR0B = 0b101;
	//TIMSK0 = _BV(OCIE0A);

	//sei();
}

void start_sd(void)
{
	disk_initialize(0);
}

int main(void)
{
	start_timer();
	start_ethernet();
	//start_serial();
	start_sd();
	httpServer_init(TX_BUF, RX_BUF, MAX_HTTPSOCK, socknumlist);

	while(1)
	{
		for(int i = 0; i < MAX_HTTPSOCK; i++)
		{
			httpServer_run(i);
		}
	}

	return 0;
}
