#include <asf.h>
#include <avr/io.h>
#include "ra915.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();

	/* Insert application code here, after the board has been initialized. */
	struct ra915struct frame = {
		.marker = 0xA5
	};
	//ra915data.data.concentration = adcdata - 0x7FFF;
	frame.checksum = genchecksum(&frame.data);
	//usart_serial_write_packet(&USARTC0, (uint8_t *)&ra915data, 23);

	//bit_is_set(CPU_SREG,CPU_V_bp);
}
