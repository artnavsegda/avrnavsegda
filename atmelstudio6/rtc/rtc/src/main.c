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
	
	//if (rtc_vbat_system_check(false) != VBAT_STATUS_OK)
	//{
	//	rtc_init();
	//}
	
	//sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_RTC);
	PR.PRGEN &= ~PR_RTC_bm;
	VBAT.CTRL = VBAT_ACCEN_bm;
	
	printf("MCU started\n\r");
	
	while (true)
	{
		printf("RTC time is %lu\n\r",rtc_get_time());
		delay_s(1);
	}
	
}
