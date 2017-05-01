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

	sysclk_init();
	board_init();
	stdio_serial_init(&USARTC0, &usart_serial_options);

	/* Insert application code here, after the board has been initialized. */
	enum vbat_status_code vbatstatus;
	vbatstatus = rtc_vbat_system_check(true);
	switch (vbatstatus)
	{
		case VBAT_STATUS_OK:
			printf("VBAT reported OK\n\r");
			break;
		case VBAT_STATUS_NO_POWER:
			printf("VBAT has no power\n\r");
			break;
		case VBAT_STATUS_BBBOD:
			printf("VBAT detected brown out while power is absent\n\r");
			break;
		case VBAT_STATUS_XOSCFAIL:
			printf("RTC crystal failed\n\r");
			break;
		default:
			break;
	}
	printf("VBAT status is %d\n\r",vbatstatus);
	while (1);
}
