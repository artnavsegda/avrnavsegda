#include <asf.h>
#include "i2c.h"
#include "pca9557.h"
#include "setup.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	gfx_mono_draw_string("unit test display",10,10,&sysfont);
}

