#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

int main(void)
{
	startserial();
	printf("MCU started\n\r");
	while(1)
	{

	}
	return 0;
}
