#include <asf.h>
#include <stdio.h>
#include "spi_transfer.h"
#include "rolling.h"
#include "interrupt.h"
#include "ad7705.h"

extern struct spi_device SPI_ADC;
extern uint16_t adcdata;
struct massive firststage, secondstage;

void ad7705_callback(void)
{
	LED_Toggle(LED3);
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
	{
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
		increment(&firststage,adcdata);
	}
}

float calculatecalibration(long averaged, long zerolevelavg, long coefficent, float standard_concentration)
{
	return (
		(long) averaged - (long) zerolevelavg
	) / (float) (
		(long) coefficent - (long) zerolevelavg
	) * standard_concentration;
}

float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor)
{
	return (
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

void tc_callback(void)
{
	LED_Toggle(LED2);
	increment(&secondstage,oversample(&firststage,64)/64);
	printf("\033[2J%6X\r", adcdata);
	printf("%6lX\r", oversample(&firststage,16)/16);
	printf("%6lX\r", oversample(&firststage,32)/32);
	printf("%6lX\r", oversample(&firststage,64)/64);
	printf("%6lX\r", oversample(&secondstage,8)/8);
	printf("ELEMENTALMERCURYROW = %d\r", calculatecalibration(oversample(&secondstage, 32), mystate->zerolevelavg, mystate->coefficent, 0.65));
	printf("TOTALMERCURYROW = %d\r", calculatecell(oversample(&secondstage, 32), mystate->zerolevelavg, mystate->celllevelavg, mystate->celllevelavg, 0.24, 0.17));
	tc_clear_overflow(&TCC0);
}
