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
}



