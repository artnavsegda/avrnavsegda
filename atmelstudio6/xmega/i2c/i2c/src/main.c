#include <asf.h>

uint8_t data_received[1];

twi_master_options_t opt = {
	  .speed = 50000,
	  .chip  = 0x20
};
  
twi_package_t packet_read = {
	  .addr_length  = 0,
	  .chip         = 0x20,
	  .buffer       = data_received,
	  .length       = 1
};

usart_serial_options_t usart_serial_options = {
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
	twi_master_setup(&TWIC, &opt);

	/* Insert application code here, after the board has been initialized. */
	while (1)
	{
		if (twi_master_read(&TWIC, &packet_read) == TWI_SUCCESS)
			printf("Data: %x\r\n",data_received[0]);
		delay_ms(1000);
	}
}
