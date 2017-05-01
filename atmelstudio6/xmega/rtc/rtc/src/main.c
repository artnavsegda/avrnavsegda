#include <asf.h>

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();
	stdio_serial_init(&USARTC0, &usart_serial_options);

	/* Insert application code here, after the board has been initialized. */
	
	switch (rtc_vbat_system_check(false))
	{
		case VBAT_STATUS_NO_POWER:
			printf("No power detected on VBAT\r\n");
			rtc_init();
		break;
		case VBAT_STATUS_INIT:
			printf("The backup system must be initialized\r\n");
			rtc_init();
		break;
		case VBAT_STATUS_BBPOR:
			printf("A POR was detected on the VBAT input\r\n");
			rtc_init();
		break;
		case VBAT_STATUS_BBBOD:
			printf("A brown-out was detected on the VBAT input\r\n");
			rtc_init();
		break;
		case VBAT_STATUS_XOSCFAIL:
			printf("A failure was detected on the oscillator\r\n");
			rtc_init();
		break;
		case VBAT_STATUS_OK:
			printf("Backup system is operating and no errors were detected\r\n");
		break;
		default:
		break;
	}
	
	printf("MCU started\n\r");
	
	while (true)
	{
		printf("RTC time is %lu\r\n",rtc_get_time());
		delay_s(1);
	}
	
}
