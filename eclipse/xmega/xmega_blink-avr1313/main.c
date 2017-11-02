#include <avr/io.h>
#include <util/delay.h>
#include "avr_compiler.h"
#include "port_driver.h"

int main(void)
{
	PORT_SetPinsAsOutput(&PORTD, _BV(4));
	while(1)
	{
		PORT_TogglePins(&PORTD, _BV(4));
		_delay_ms(1000);
	}
}
