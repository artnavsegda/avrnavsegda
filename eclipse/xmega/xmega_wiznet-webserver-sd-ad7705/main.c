#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "wizchip_conf.h"
#include "serial.h"
#include "ff.h"
#include "diskio.h"

uint8_t RX_BUF[1024];
uint8_t TX_BUF[1024];
uint8_t socknumlist[] = {0, 1, 2, 3};

ISR(TCC0_OVF_vect)
{
	disk_timerproc();       /* Drive timer procedure of low level disk I/O module */
	PORTD.OUTTGL = PIN4_bm;
}

ISR(TCC1_OVF_vect)
{
	httpServer_time_handler();
	PORTD.OUTTGL = PIN5_bm;
}

static void  wizchip_select(void)
{
	PORTB.OUTSET = _BV(7);
	PORTC.OUTCLR = _BV(0);
}

static void  wizchip_deselect(void)
{
	PORTC.OUTSET = _BV(0);
	PORTB.OUTCLR = _BV(7);
}

char spi_transfer(char c)
{
	SPIC.DATA = c;
	loop_until_bit_is_set(SPIC.STATUS,SPI_IF_bp);
	return SPIC.DATA;
}

static uint8_t spi_read()
{
	return spi_transfer(0xFF);
}

static void spi_write(uint8_t wb)
{
	spi_transfer(wb);
}

static void spi_array_write(uint8_t* pBuf, uint16_t len)
{
        for (int i = 0; i < len; i++)
        		spi_write(pBuf[i]);
}

static void spi_array_read(uint8_t* pBuf, uint16_t len)
{
        for (int i = 0; i < len; i++)
        		pBuf[i] = spi_read();
}

unsigned int adcresult;

uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1; // ret = 1 means 'uri_name' matched
	if(strcmp((const char *)uri_name, "hello.cgi") == 0)
		*len = sprintf((char *)buf, "hello world");
	if(strcmp((const char *)uri_name, "adcdata.cgi") == 0)
		*len = sprintf((char *)buf, "%04X", __builtin_bswap16(adcresult));
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
	                            .ip 	= {192, 168, 1, 150},					// IP address
	                            .sn 	= {255, 255, 0, 0},					// Subnet mask
	                            .gw 	= {192, 168, 0, 1},						// Gateway address
	                            .dns 	= {8, 8, 8, 8},							// DNS server
	                            .dhcp 	= NETINFO_STATIC };						// DHCP enable / disable

	uint8_t memsize[2][4] = { { 2, 2, 2, 2 }, { 2, 2, 2, 2 } };
	reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
	reg_wizchip_spi_cbfunc(spi_read, spi_write);
	reg_wizchip_spiburst_cbfunc(spi_array_read, spi_array_write);
	ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize);
	ctlnetwork(CN_SET_NETINFO, (void*) &netinfo);
}

void start_timer(void)
{
	TCC0_PER = 20000;                // Set period 10000
	TCC0_CTRLA = TC_CLKSEL_DIV1_gc;                // Prescaler DIV1
	TCC0_INTCTRLA = 2;                // Enable overflow interrupt
	// 31250 * 64 = 2000000 = 2 mHz
	TCC1_PER = 31250;                // Set period 31250
	TCC1_CTRLA = TC_CLKSEL_DIV64_gc;                // Prescaler DIV64
	TCC1_INTCTRLA = 2;                // Enable overflow interrupt
	PMIC_CTRL = 2;                    // Enable medium level interrupts
	sei();
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

void ad7705_init(void)
{
	// Activate the CS pin
	PORTC.OUTCLR = PIN4_bm;
	spi_array_write("\xFF\xFF\xFF\xFF\xFF", 5);
	_delay_ms(10);
	spi_array_write("\x20\x0C", 2);
	_delay_ms(10);
	spi_array_write("\x10\x04", 2);
	_delay_ms(10);
	spi_array_write("\x60\x18\x3A\x00", 4);
	_delay_ms(10);
	spi_array_write("\x70\x89\x78\xD7", 4);
	_delay_ms(10);
	// CS pin is not active
	PORTC.OUTSET = PIN4_bm;
}

int main(void)
{
	FATFS fs;
	uint8_t tmpstr[6] = {0,};

	start_serial();
	printf("Serial started\r\n");
	start_timer();
	printf("Timer started\r\n");

	// Initial the Peripheral
	// Set MOSI (PORTC5),SCK (PORTC7), PORTC0 (ETH SS) and PORTC4 (adc SS) as output, others as input
	PORTC.DIRSET = PIN5_bm|PIN7_bm|PIN4_bm|PIN0_bm;
	// Set PORTE2 (sdcard SS) as output
	PORTE.DIRSET = PIN2_bm;
	// Set PORTA0 (eth reset) as output
	PORTA.DIRSET = PIN0_bm;
	// SEN wiznet
	PORTB.DIRSET = PIN7_bm;
	// status/power LED
	PORTD.DIRSET = PIN4_bm | PIN5_bm;

	// adc SS and eth SS deactivate
	PORTC.OUTSET = PIN4_bm|PIN0_bm;
	// sdcard ss deactivate
	PORTE.OUTSET = PIN2_bm;
	// disable wiznet spi
	PORTB.OUTCLR = PIN7_bm;

	// MISO level pull up
	PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
	// Enable SPI, Master Mode 0, set the clock rate fck/2
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm;

	// Reset and check sd card
	f_mount(&fs, "", 0);
	printf("SD card started\r\n");

	// init ad7705
	ad7705_init();

	// Reset W5100
	PORTA.OUTCLR = PIN0_bm; //write zero
	_delay_ms(1);
	PORTA.OUTSET = PIN0_bm; //write one

	start_ethernet();
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	printf("Wiznet %s started\r\n", tmpstr);
	Display_Net_Conf();
	httpServer_init(TX_BUF, RX_BUF, 4, socknumlist);

	while(1)
	{
		if (bit_is_clear(PORTC.IN,PIN1_bp))
		{
			PORTC.OUTCLR = PIN4_bm;
			spi_write(0x38);
			spi_array_read((char *)&adcresult,2);
			PORTC.OUTSET = PIN4_bm;
			PORTD.OUTTGL = PIN4_bm;
		}
		for(int i = 0; i < 4; i++)
		{
			httpServer_run(i);
		}
	}

	return 0;
}
