#include <asf.h>
#include <stdio.h>
#include "setup.h"

extern int16_t adca_scan_results[8];
extern int16_t adcb_scan_results[8];

#define MASSIVE_SIZE 1000

struct massive {
	uint16_t massive[MASSIVE_SIZE];
	uint16_t position;
};

long average(struct massive working, uint16_t amount)
{
	long x = 0;
	for(int i=0; i<amount; i++)
	{
		if (working.position-i>0)
			x=x+working.massive[working.position-i-1];
		else
			x=x+working.massive[sizeof(working.massive)+(working.position-i)-1];
	}
	return x;
}

void iterate(struct massive working, uint16_t value)
{
	working.massive[working.position] = value;
	if (working.position++ > sizeof(working.massive))
		working.position = 0;
}

int main (void)
{
	char string[20];
	struct massive adcone;

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		iterate(adcone, adca_scan_results[0]);
		snprintf(string,sizeof(string),"%d", average(adcone, 500));
		gfx_mono_draw_string(string,8,0,&sysfont);
		delay_ms(1000);
	}
}
