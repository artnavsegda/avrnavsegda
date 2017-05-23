#include <asf.h>
#include "setup.h"

ISR(USARTC0_RXC_vect)
{
	LED_Toggle(LED0);
	usart_putchar(&USARTC0, usart_getchar(&USARTC0));
	usart_clear_rx_complete(&USARTC0);
	while (!usart_tx_is_complete(&USARTC0)) {
	}
	usart_clear_tx_complete(&USARTC0);
}

int main (void)
{
	//uint8_t frame[5];
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	printf("\n\rHello ATMEL World!\n\r");

	do {
		if (usart_serial_read_packet(&USARTC0,(uint8_t *)&frame,5) == STATUS_OK)
		//	LED_Toggle(LED0);
	} while (1);
}
