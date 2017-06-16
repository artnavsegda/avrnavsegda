#include <asf.h>

static void my_callback(void)
{
	// User code to execute when the overflow occurs here
	//Important to clear Interrupt Flag
	tc_clear_overflow(&TCC0);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	pmic_init();
	sysclk_init();
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, my_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 1000);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	cpu_irq_enable();
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1_gc);

	/* Insert application code here, after the board has been initialized. */
	while (1);
}
