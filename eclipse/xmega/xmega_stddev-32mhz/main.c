#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

float calculateSD(float data[])
{
    float sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i=0; i<100; ++i)
    {
        sum += data[i];
    }

    mean = sum/100;

    for(i=0; i<100; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation/100);
}

float average(int amount)
{
	unsigned long temporary = 0;
	for (int i=0; i<amount;i++)
	{
		ADCB.CH0.CTRL |= ADC_CH_START_bm;
		while(!ADCB.CH0.INTFLAGS);
		temporary += ADCB.CH0RES;
	}
	return temporary/amount;
}

int main(void)
{
	float result[100];

    OSC.CTRL = 0x02;
    while(!(OSC.STATUS & OSC_RC32MRDY_bm));
    CPU_CCP = 0xD8;
    CLK.CTRL = 1;

	startserial(207);//9600
	printf("MCU started\n\r");

	ADCB.CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCB.PRESCALER |= ADC_PRESCALER_DIV64_gc;
	ADCB.REFCTRL |= ADC_REFSEL_INTVCC_gc;
	ADCB.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCB.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN3_gc;
	ADCB.CTRLA |= ADC_ENABLE_bm;

	while(1)
	{
		for (int i = 0; i < 100; i++)
		{
			ADCB.CH0.CTRL |= ADC_CH_START_bm;
			while(!ADCB.CH0.INTFLAGS);
			result[i] = average(64);
		}
		printf("Standard Deviation = %.6f\n\r", calculateSD(result));
		_delay_ms(100);
	}
	return 0;
}
