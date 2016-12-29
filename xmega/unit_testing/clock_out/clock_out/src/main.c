#include <asf.h>

#define CLKOUT_PORT     PORTCFG_CLKOUT_PC7_gc
#define IOPORT_PIN      IOPORT_CREATE_PIN(PORTC, 7)

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();

	/* Insert application code here, after the board has been initialized. */

	ioport_configure_pin(IOPORT_PIN, IOPORT_DIR_OUTPUT);
	PORTCFG.CLKEVOUT = CLKOUT_PORT;

	while (1) {
		/* Do nothing */
	}

}
