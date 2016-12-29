#include <asf.h>

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();

	/* Insert application code here, after the board has been initialized. */
	while (1)
	{
		LED_Toggle(LED0);
		delay_s(1);
	}
}
