#include <asf.h>
#include "setup.h"
#include "pca9557.h"
#include "ra915.h"

int main (void)
{
	int packetsize;
	int marker;
	uint8_t buffer[100];

	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	do {
		/* Go to sleep, everything is handled by interrupts. */
		//sleepmgr_enter_sleep();
		marker = udi_cdc_getc();
		switch (marker)
		{
			case 0xB5:
				packetsize = 4;
				break;
			case 0xCA:
				packetsize = 2;
				break;
			default:
				packetsize = 0;
				break;
		}
		if (packetsize != 0)
		{
			udi_cdc_read_buf(buffer,packetsize);
			if (buffer[packetsize-1]==genchecksum(buffer,packetsize))
				process_ra915_request(marker,buffer,packetsize);
		}

		/*if (strchr(markers,marker)!=NULL)
		{
			udi_cdc_read_buf(buffer,packetsize(marker));
			if (buffer[packetsize(marker)-1] == genchecksum(buffer,packetsize(marker)))
			{
				process_ra915_request(marker,buffer,packetsize(marker))
			}
		}*/
	} while (1);
}
