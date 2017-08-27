#include <asf.h>
#include <stdio.h>

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);

const usart_rs232_options_t usart_serial_options = {
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
	usart_init_rs232(&USART0, &usart_serial_options);
	stdout = &mystdout;

	/* Insert application code here, after the board has been initialized. */
	printf("Hello World!\n\r");
	
	while(1)
	{
		delay_s(1);
		printf("hello\n\r");
	}
}
