#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "pca9557.h"
#include "rolling.h"

struct pca9557_pin x20_relay = { .address = 0x18, .pin_number = 0 };
struct pca9557_pin x19_relay = { .address = 0x19, .pin_number = 0 };
uint16_t zerolevelavg, coefficent;
extern struct massive firststage, secondstage;

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	setup_init();
	setup_configure();
	setup_enable();

	pca9557_set_pin_dir(x19_relay.address, x19_relay.pin_number, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(x20_relay.address, x20_relay.pin_number, PCA9557_DIR_OUTPUT);

	/* Insert application code here, after the board has been initialized. */
	do {
		pca9557_set_pin_level(x19_relay.address, x19_relay.pin_number, false);
		LED_On(LED2);
		delay_s(30);
		zerolevelavg = oversample(&secondstage,30)/30;
		printf("zerolevelavg is %u\n\r", zerolevelavg);

		pca9557_set_pin_level(x19_relay.address, x19_relay.pin_number, true);
		LED_Off(LED2);
		delay_s(30);

		pca9557_set_pin_level(x20_relay.address, x20_relay.pin_number, false);
		LED_On(LED3);
		delay_s(30);
		coefficent = oversample(&secondstage,30)/30;
		printf("coefficent is %u\n\r", coefficent);

		pca9557_set_pin_level(x20_relay.address, x20_relay.pin_number, true);
		LED_Off(LED3);
		delay_s(30);
	} while (true);
}
