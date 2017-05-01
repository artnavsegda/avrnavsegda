/*
 * naked_blink.c
 *
 * Created: 4/9/2017 2:43:40 PM
 *  Author: Art Navsegda
 */ 

#define F_CPU 2000000UL  // 2 MHz

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	PORTD_DIRSET = PIN4_bm;
    while(1)
    {
        //TODO:: Please write your application code 
		PORTD_OUTTGL = PIN4_bm;
		_delay_ms(1000);
    }
}