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