#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"

void adc_init()
{
	// AREF = AVcc
	ADMUX = _BV(REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = _BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
}

uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with ’7? will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1? to ADSC
	ADCSRA |= _BV(ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0? again
	// till then, run loop continuously
	while(ADCSRA & _BV(ADSC));
	
	return (ADC);
}

int main(void)
{
	startserial();
	adc_init();

	while(1)
	{
		_delay_ms(100);
		printf("%x\r\n",adc_read(0));
	}
	return 0;
}
