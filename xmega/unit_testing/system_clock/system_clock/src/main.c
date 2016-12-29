#include <asf.h>

#define CLKOUT_PORT     PORTCFG_CLKOUT_PC7_gc
#define IOPORT_PIN      IOPORT_CREATE_PIN(PORTC, 7)

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	// Configure clock to 32MHz
	OSC.CTRL |= OSC_RC32MEN_bm | OSC_RC32KEN_bm;  /* Enable the internal 32MHz & 32KHz oscillators */
	while(!(OSC.STATUS & OSC_RC32KRDY_bm));       /* Wait for 32Khz oscillator to stabilize */
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));       /* Wait for 32MHz oscillator to stabilize */
	DFLLRC32M.CTRL = DFLL_ENABLE_bm ;             /* Enable DFLL - defaults to calibrate against internal 32Khz clock */
	CCP = CCP_IOREG_gc;                           /* Disable register security for clock update */
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;              /* Switch to 32MHz clock */
	OSC.CTRL &= ~OSC_RC2MEN_bm;                   /* Disable 2Mhz oscillator */

	board_init();

	/* Insert application code here, after the board has been initialized. */

	ioport_configure_pin(IOPORT_PIN, IOPORT_DIR_OUTPUT);
	PORTCFG.CLKEVOUT = CLKOUT_PORT;

	while (1) {
		/* Do nothing */
	}

}
