#include <asf.h>
#include "setup.h"
#include <avr/io.h>
#include "ra915.h"
#include "pca9557.h"

int main (void)
{
	uint8_t frame[5];
	uint8_t controlbyte;
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();
	//ra915init();

	/* Insert application code here, after the board has been initialized. */
	//do nothing as timer counter working inside interrupt itself
	/*delay_ms(100);
	pca9557_set_pin_dir(0x1a, 0, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_level(0x1a, 0, false);
	pca9557_set_pin_dir(0x1a, 1, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_level(0x1a, 1, true);*/
	do {
		/* Go to sleep, everything is handled by interrupts. */
		//sleepmgr_enter_sleep();
		/*if (usart_serial_read_packet(&USARTC0,(uint8_t *)&frame,5) == STATUS_OK)
		{
			if (frame[0] == 0xB5)
			{
				usart_serial_putchar(&USARTC0,0xB5);
				controlbyte = frame[1];
				processcontrolbyte(controlbyte);
			}
		}*/
	} while (1);
}
