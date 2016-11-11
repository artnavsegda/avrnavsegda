#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "pca9557.h"

struct pca9557_pin x20_relay = { .address = 0x18, .pin_number = 0 };
struct pca9557_pin x19_relay = { .address = 0x19, .pin_number = 0 };
extern uint16_t adcdata;
uint16_t adczero, adccal;

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
		delay_s(1);
		adczero = adcdata;
		pca9557_set_pin_level(x19_relay.address, x19_relay.pin_number, false);
		LED_On(LED2);

		delay_s(1);
		pca9557_set_pin_level(x19_relay.address, x19_relay.pin_number, true);
		LED_Off(LED2);

		delay_s(1);
		adccal = adcdata;
		pca9557_set_pin_level(x20_relay.address, x20_relay.pin_number, false);
		LED_On(LED3);

		delay_s(1);
		pca9557_set_pin_level(x20_relay.address, x20_relay.pin_number, true);
		LED_Off(LED3);

	} while (true);
}
