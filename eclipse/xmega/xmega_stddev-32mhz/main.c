#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

#define SAMPLERATE ADC_PRESCALER_DIV8_gc
#define SAMPLES 50
#define AVERAGING 64

#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

float calculateSD(float data[])
{
    float sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i=0; i<SAMPLES; ++i)
    {
        sum += data[i];
    }

    mean = sum/SAMPLES;

    for(i=0; i<SAMPLES; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    printf("Mean = %.0f\n\r", mean);

    return sqrt(standardDeviation/SAMPLES);
}

float average(int amount)
{
	unsigned long temporary = 0;
	for (int i=0; i<amount;i++)
	{
		ADCB.CH0.CTRL |= ADC_CH_START_bm;
		while(!ADCB.CH0.INTFLAGS);
		temporary += ADCB.CH0RES;
		//_delay_us(100);
	}
	return temporary/amount;
}

int main(void)
{
	float result[SAMPLES];

    OSC.CTRL = 0x02;
    while(!(OSC.STATUS & OSC_RC32MRDY_bm));
    CPU_CCP = 0xD8;
    CLK.CTRL = 1;

	startserial(34);//9600
	printf("MCU started\n\r");

	ADCB.CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCB.PRESCALER |= SAMPLERATE;
	ADCB.REFCTRL |= ADC_REFSEL_INTVCC_gc;
	ADCB.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCB.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN3_gc;
	ADCB.CTRLA |= ADC_ENABLE_bm;

	while(1)
	{
		printf("Sample rate "STRINGIFY(SAMPLERATE)", "STRINGIFY(SAMPLES)" samples, averaging "STRINGIFY(AVERAGING)"\n\r");
		for (int i = 0; i < 100; i++)
		{
			//ADCB.CH0.CTRL |= ADC_CH_START_bm;
			//while(!ADCB.CH0.INTFLAGS);
			//result[i] = ADCB.CH0RES;
			result[i] = average(AVERAGING);
		}
		for (int i = 0; i < SAMPLES; i++)
		{
			printf("%.0f\n\r", result[i]);
		}
		printf("Standard Deviation = %.6f\n\r", calculateSD(result));
		_delay_ms(1000);
	}
	return 0;
}
