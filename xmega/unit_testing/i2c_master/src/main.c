#include <asf.h>
#include "i2c.h"
#include "setup.h"

struct MyObject {
	char zero[4];
	char one[6];
	char two[26];
	char three[13];
};

struct MyObject e;

struct MyObject d = {
	"4321",
	"654321",
	"abcdefghijklmnopqrstuwxyz",
	"0123456789abc"
};

struct MyObject c;

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	i2c_read_array(&TWIE, 0x08, 0, 4, e.zero);
	i2c_read_array(&TWIE, 0x08, 1, 6, e.one);
	i2c_read_array(&TWIE, 0x08, 2, 26, e.two);
	i2c_read_array(&TWIE, 0x08, 3, 13, e.three);

	i2c_send_array(&TWIE, 0x08, 0, 4, d.zero);
	i2c_send_array(&TWIE, 0x08, 1, 6, d.one);
	i2c_send_array(&TWIE, 0x08, 2, 26, d.two);
	i2c_send_array(&TWIE, 0x08, 3, 13, d.three);

	i2c_read_array(&TWIE, 0x08, 0, 4, c.zero);
	i2c_read_array(&TWIE, 0x08, 1, 6, c.one);
	i2c_read_array(&TWIE, 0x08, 2, 26, c.two);
	i2c_read_array(&TWIE, 0x08, 3, 13, c.three);
}
