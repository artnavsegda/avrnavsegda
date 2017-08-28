#include <asf.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

const usart_rs232_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

static int uart_putchar(char c, FILE *stream)
{
	usart_putchar(&USART0, c);
	return 0;
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	//sysclk_init();
	//board_init();
	usart_init_rs232(&USART0, &usart_serial_options);
	usart_set_baudrate(&USART0, 9600, 16000000);
	stdout = &mystdout;

	adc_init(ADC_PRESCALER_DIV128);

	/* Insert application code here, after the board has been initialized. */
	printf("MCU started\n\r");
	
	while(1)
	{
		delay_s(1);
		printf("adc value %d\n\r", adc_read_10bit(ADC_MUX_ADC0, ADC_VREF_AVCC));
	}
}
