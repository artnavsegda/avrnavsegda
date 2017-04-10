/*
 * vbat_naked.c
 *
 * Created: 4/9/2017 8:34:57 PM
 *  Author: Art Navsegda
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>

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

void init_vbat(void)
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
		VBAT.CTRL = VBAT_ACCEN_bm;
	}
}

int main(void)
{
	init_serial();
	printf("MCU started\n\r");
	init_vbat();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}