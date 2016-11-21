#include <asf.h>
#include <math.h>
#include "i2c.h"
#include "drv8832.h"
#include "setup.h"

float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor);

extern struct drv8832 cell;
extern int16_t adc_scan_results[16];
extern uint16_t adcdata;

float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor)
{
	return (float)(
			(long) averaged - (long) zerolevelavg
	) / (float) (
		(long) celllevelavg - (long) zerolevelavg
	) * (
		c_twentie_five * exp (
			kfactor * (
				(
					(
						(
							(
								celltempavg - 180 // ADC zero level
							) * (
								(
									3.3 / 1.6 // Voltage reference
								) / 4095 // ADC resolution
							)
						) - 0.5
					) * 100.0 // temperature in Celsius
				) - 25.0
			)
		)
	);
}

int main (void)
{
	uint16_t callvalue, celltemp, zerovalue;
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	drv8832_init(cell);
	//while (true)
	//{
		delay_s(10);
		zerovalue = adcdata;
		drv8832_turn(cell, DRV8832_LEFT);
		delay_s(10);
		callvalue = adcdata;
		celltemp = adc_scan_results[6];
		drv8832_turn(cell, DRV8832_RIGHT);
		delay_s(10);
		float i = calculatecell(adcdata,zerovalue,callvalue,celltemp,1.0,1.0);
	//}
}
