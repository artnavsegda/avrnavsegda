#include <asf.h>
#include "setup.h"

int irqcount = 0;
uint8_t buffer[100];

void my_callback_rx_notify(uint8_t port)
{
	LED_Toggle(LED0);
	irqcount++;
	int length = udi_cdc_get_nb_received_data();
	printf("IRQ:%d:length=%d ",irqcount, length);
	/*while (udi_cdc_is_rx_ready())
	printf(" 0x%X", udi_cdc_getc());
	printf("\n\r");*/

	udi_cdc_read_buf(buffer,length);
	for (int i=0;i<length;i++)
	{
		printf(" 0x%02X", buffer[i]);
	}
	printf("\n\r");

	if(length == 8)
	{
		crc_set_initial_value(0xFFFF);
		printf("CRC calculation: %04X\n\r", crc_io_checksum(&buffer,6,CRC_16BIT));
	}

}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		sleepmgr_enter_sleep();
	}
}
