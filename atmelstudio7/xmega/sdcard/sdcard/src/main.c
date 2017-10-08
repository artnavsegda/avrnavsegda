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
	printf("serial start\n\r");
	sd_mmc_init();
	printf("sd mmc start\n\r");

	/* Insert application code here, after the board has been initialized. */
}
