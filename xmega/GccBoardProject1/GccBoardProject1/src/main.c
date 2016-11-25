#include <asf.h>
#include "setup.h"
#include <avr/io.h>
#include "ra915.h"

int main (void)
{
	uint8_t frame[5];
	uint8_t controlbyte;
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	//do nothing as timer counter working inside interrupt itself
	do {
		/* Go to sleep, everything is handled by interrupts. */
		//sleepmgr_enter_sleep();
		if (usart_serial_read_packet(&USARTC0,(uint8_t *)&frame,5) == STATUS_OK)
		{
			if (frame[0] == 0xB5)
			{
				usart_serial_putchar(&USARTC0,0xB5);
				controlbyte = frame[1];
				processcontrolbyte(controlbyte);
			}
		}
	} while (1);
}
