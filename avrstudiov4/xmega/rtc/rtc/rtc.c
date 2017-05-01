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

void rtc_init(void)
{
	//PR.PRGEN &= ~PR_RTC_bm; // enable sysclock rtc
	// Set up VBAT system and start oscillator
	init_vbat();

	// Disable the RTC32 module before setting it up
	RTC32.CTRL = 0;

	while (RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm);

	// Set up maximum period and start at 0
	RTC32.PER = 0xffffffff;
	RTC32.CNT = 0;

	while (RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm);

	RTC32.INTCTRL = 0;
	RTC32.CTRL = RTC32_ENABLE_bm;

	// Make sure it's sync'ed before return
	while (RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm);
}

void status_vbat(void)
{
	PR.PRGEN &= ~PR_RTC_bm; // enable sysclock rtc
	if (VBAT.STATUS & VBAT_BBPWR_bm)
		printf("VBAT has no power\n\r");
	else if (VBAT.STATUS & VBAT_XOSCFAIL_bm)
		printf("RTC crystal oscillator failed\n\r");
	else if (VBAT.STATUS & VBAT_BBBORF_bm)
		printf("VBAT had brownout while no power is present\n\r");
	else if (VBAT.STATUS & VBAT_BBPORF_bm)
		printf("VBAT had power-on reset\n\r");
	else
	{
		printf("VBAT OK\n\r");
		VBAT.CTRL = VBAT_ACCEN_bm;
		return;
	}
	rtc_init();
}

uint32_t rtc_get_time(void)
{
	RTC32.SYNCCTRL = RTC32_SYNCCNT_bm;
	while (RTC32.SYNCCTRL & RTC32_SYNCCNT_bm);
	return RTC32.CNT;
}

int main(void)
{
	init_serial();
	printf("MCU started\n\r");
	status_vbat();
    while(1)
    {
		printf("RTC time is %lu\n\r",rtc_get_time());
		_delay_ms(1000);
   }
}
