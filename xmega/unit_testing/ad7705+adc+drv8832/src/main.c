#include <asf.h>
#include "i2c.h"
#include "drv8832.h"
#include "setup.h"

extern struct drv8832 cell;
extern struct drv8832 servo_2;

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	drv8832_init(cell);
	drv8832_init(servo_2);
	while (true)
	{
		drv8832_turn(cell, DRV8832_LEFT);
		delay_ms(1000);
		drv8832_turn(cell, DRV8832_RIGHT);
		delay_ms(1000);
		drv8832_turn(servo_2, DRV8832_LEFT);
		delay_ms(1000);
		drv8832_turn(servo_2, DRV8832_RIGHT);
		delay_ms(1000);
	}

}
