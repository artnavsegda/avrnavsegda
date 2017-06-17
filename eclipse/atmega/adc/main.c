#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"

int readadc(char chan)
{
	ADMUX = _BV(REFS0);
}

int main(void)
{
	startserial();

	ADMUX = _BV(REFS0);
	ADCSRA = _BV(ADEN) | 7;

	printf("ADC started\n\r");

    while(1)
    {
        //TODO:: Please write your application code
    }

	return 0;
}

