#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"

int main()
{
	startserial();
	printf("Hello world\r\n");

	while(1);
}
