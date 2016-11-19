#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "pca9557.h"
#include "rolling.h"

struct pca9557_pin x20_relay = { .address = 0x18, .pin_number = 0 };
struct pca9557_pin x19_relay = { .address = 0x19, .pin_number = 0 };
uint16_t zerolevelavg, coefficent, measurment;
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
	pca9557_set_pin_high(x19_relay.address, x19_relay.pin_number);
	LED_Off(LED2);

	pca9557_set_pin_high(x20_relay.address, x20_relay.pin_number);
	LED_Off(LED3);

	printf("\n\rHello ATMEL World!\n\r");
	delay_s(10);


	do {
		printf("enabling green relay\n\r");
		pca9557_set_pin_low(x19_relay.address, x19_relay.pin_number);
		LED_On(LED3);
		delay_s(10);
		zerolevelavg = oversample(&secondstage,10)/10;
		printf("zerolevelavg is %u\n\r", zerolevelavg);

		printf("disabling green relay\n\r");
		pca9557_set_pin_high(x19_relay.address, x19_relay.pin_number);
		LED_Off(LED3);
		delay_s(10);
		measurment = oversample(&secondstage,10)/10;
		printf("measurment is %u\n\r", measurment);

		printf("enabling red relay\n\r");
		pca9557_set_pin_low(x20_relay.address, x20_relay.pin_number);
		LED_On(LED2);
		delay_s(10);
		coefficent = oversample(&secondstage,10)/10;
		printf("coefficent is %u\n\r", coefficent);

		printf("disabling green relay\n\r");
		pca9557_set_pin_high(x20_relay.address, x20_relay.pin_number);
		LED_Off(LED2);
		delay_s(10);
		measurment = oversample(&secondstage,10)/10;
		printf("measurment is %u\n\r", measurment);
		printf("calculated value is %f\n\r", (float)(measurment-zerolevelavg)/(coefficent-zerolevelavg));
	} while (true);
}
