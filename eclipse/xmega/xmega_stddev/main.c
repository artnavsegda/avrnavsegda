#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

float calculateSD(float data[])
{
    float sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i=0; i<10; ++i)
    {
        sum += data[i];
    }

    mean = sum/10;

    for(i=0; i<10; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation/10);
}

int main(void)
{
	int16_t result[10];
	startserial();
	printf("MCU started\n\r");
	ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCA.PRESCALER |= ADC_PRESCALER_DIV4_gc;
	ADCA.REFCTRL |= ADC_REFSEL_INTVCC_gc;
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CTRLA |= ADC_ENABLE_bm;

	while(1)
	{
		for (int i = 0; i < 10; i++)
		{
			ADCA.CH0.CTRL |= ADC_CH_START_bm;
			while(!ADCA.CH0.INTFLAGS);
			result[i] = ADCA.CH0RES;
		}
		printf("\nStandard Deviation = %.6f", calculateSD(result));
	}
	return 0;
}
