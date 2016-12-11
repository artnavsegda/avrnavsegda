#include <asf.h>
#include "setup.h"

uint8_t buffer[100];

void my_callback_rx_notify(uint8_t port)
{
	int i = udi_cdc_get_nb_received_data();
	udi_cdc_read_buf(buffer,i);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
}
