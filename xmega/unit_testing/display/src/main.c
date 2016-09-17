#include <asf.h>

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();

	/* Insert application code here, after the board has been initialized. */
	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	gfx_mono_draw_string("unit test display",10,10,&sysfont);

	while (true)
	{
		gfx_mono_draw_string("do something funny",10,10,&sysfont);
		delay_ms(500);
	}

}
