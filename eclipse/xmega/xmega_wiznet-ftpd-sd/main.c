#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "wizchip_conf.h"
#include "serial.h"
#include "ff.h"
#include "diskio.h"
#include "ftpd.h"

uint8_t gFTPBUF[4096];

ISR(TCC0_OVF_vect)
{
	disk_timerproc();       /* Drive timer procedure of low level disk I/O module */
	PORTD.OUTTGL = PIN4_bm;
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

static uint8_t wizchip_read()
{
	uint8_t rb;
	SPIC.DATA = 0xFF;
	loop_until_bit_is_set(SPIC.STATUS, SPI_IF_bp);
	rb = SPIC.DATA;
	return rb;
}

static void  wizchip_write(uint8_t wb)
{
	SPIC.DATA = wb;
	loop_until_bit_is_set(SPIC.STATUS, SPI_IF_bp);
}

static void wizchip_readburst(uint8_t* pBuf, uint16_t len)
{
	for (int i = 0; i < len; i++)
	{
		SPIC.DATA = 0xFF;
		loop_until_bit_is_set(SPIC.STATUS, SPI_IF_bp);
		pBuf[i] = SPIC.DATA;
	}
}

static void  wizchip_writeburst(uint8_t* pBuf, uint16_t len)
{
	for (int i = 0; i < len; i++)
	{
		SPIC.DATA = pBuf[i];
		loop_until_bit_is_set(SPIC.STATUS, SPI_IF_bp);
	}
}

void start_ethernet(wiz_NetInfo netinfo)
{
	uint8_t memsize[2][4] = { { 2, 2, 2, 2 }, { 2, 2, 2, 2 } };
	reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
	reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
	reg_wizchip_spiburst_cbfunc(wizchip_readburst, wizchip_writeburst);
	ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize);
	ctlnetwork(CN_SET_NETINFO, (void*) &netinfo);
}

void start_timer(void)
{
	TCC0_PER = 20000;                // Set period 10000
	TCC0_CTRLA = TC_CLKSEL_DIV1_gc;                // Prescaler DIV1
	TCC0_INTCTRLA = 2;                // Enable overflow interrupt
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

int main(void)
{
	wiz_NetInfo gWIZNETINFO = { .mac 	= {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},	// Mac address
	                            .ip 	= {192, 168, 1, 150},					// IP address
	                            .sn 	= {255, 255, 0, 0},					// Subnet mask
	                            .gw 	= {192, 168, 0, 1},						// Gateway address
	                            .dns 	= {8, 8, 8, 8},							// DNS server
	                            .dhcp 	= NETINFO_STATIC };						// DHCP enable / disable

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

	// Reset W5100
	PORTA.OUTCLR = PIN0_bm; //write zero
	_delay_ms(1);
	PORTA.OUTSET = PIN0_bm; //write one

	start_ethernet(gWIZNETINFO);
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	printf("Wiznet %s started\r\n", tmpstr);
	Display_Net_Conf();
	ftpd_init(gWIZNETINFO.ip);

	while(1)
	{
		ftpd_run(gFTPBUF);
	}

	return 0;
}
