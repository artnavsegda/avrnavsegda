#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"
#include "i2c.h"

int main(void)
{
	startserial();
	printf("mcu started\n\r");
	starti2c();
	pcawrite(0x18,0x02,0x00);//reset inversion
	pcawrite(0x19,0x02,0x00);//reset inversion
	pcawrite(0x1a,0x02,0x00);//reset inversion

	pcawrite(0x1a,0x03,!_BV(1));//set ignition as output
	pcawrite(0x1a,0x01,_BV(1));//enable ignition
	_delay_ms(1000);
	pcawrite(0x1a,0x01,!_BV(1));//enable ignition


}



