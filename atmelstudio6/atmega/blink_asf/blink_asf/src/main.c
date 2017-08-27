#include <asf.h>

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	ioport_configure_pin(ARDUINO13, IOPORT_DIR_OUTPUT);

	/* Insert application code here, after the board has been initialized. */
	while(1)
	{
		ioport_toggle_pin(ARDUINO13);
		delay_s(1);
	}
}
