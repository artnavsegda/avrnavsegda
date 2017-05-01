/*
 * vbat_naked.c
 *
 * Created: 4/9/2017 8:34:57 PM
 *  Author: Art Navsegda
 */ 

#define F_CPU 2000000UL  // 2 MHz

#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(USARTC0.STATUS,USART_DREIF_bp);
	USARTC0.DATA = c;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void init_serial(void)
{
	PORTC.DIRSET = PIN3_bm;
	PORTC.DIRCLR = PIN2_bm;
	USARTC0.CTRLC = (uint8_t) USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | false;
	USARTC0.BAUDCTRLA = 12;
	USARTC0.CTRLB |= USART_RXEN_bm;
	USARTC0.CTRLB |= USART_TXEN_bm;
	stdout = &mystdout;
}

void check_vbat(void)
{
	PR.PRGEN &= ~PR_RTC_bm; // enable sysclock rtc
	if (VBAT.STATUS & VBAT_BBPWR_bm)
	{
		printf("VBAT has no power\n\r");
		return;
	}
	else if (VBAT.STATUS & VBAT_XOSCFAIL_bm)
	{
		printf("RTC crystal oscillator failed\n\r");
		return;
	}
	else if (VBAT.STATUS & VBAT_BBBORF_bm)
	{
		printf("VBAT had brownout while no power is present\n\r");
		return;
	}
	else if (VBAT.STATUS & VBAT_BBPORF_bm)
	{
		printf("VBAT had power-on reset\n\r");
		return;
	}
	else
	{
		printf("VBAT OK\n\r");
		VBAT.CTRL |= VBAT_ACCEN_bm;
	}
}

void init_vbat(void)
{
	// Enable access to VBAT
	VBAT.CTRL |= VBAT_ACCEN_bm;

	CPU_CCP = 0xD8;
	VBAT.CTRL |= VBAT_RESET_bm;

	VBAT.CTRL |= VBAT_XOSCFDEN_bm;
	/* This delay is needed to give the voltage in the backup system some
	* time to stabilize before we turn on the oscillator. If we do not
	* have this delay we may get a failure detection.
	*/
	_delay_ms(2);
	VBAT.CTRL |= VBAT_XOSCEN_bm;
	while (!(VBAT.STATUS & VBAT_XOSCRDY_bm));
}

int main(void)
{
	init_serial();
	printf("MCU started\n\r");
	check_vbat();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}