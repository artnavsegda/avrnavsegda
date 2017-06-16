#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"

char ModuleAddress = 32;

int main(void)
{
	startserial();
	//twi init
	TWSR = 0x00;
	TWBR = 0x0C;
	TWCR = _BV(TWEN);
	while (1)
	{
		//twi start
		TWCR = _BV(TWINT)|_BV(TWSTA)|_BV(TWEN);
		loop_until_bit_is_set(TWCR, TWINT);
		//twi write
		TWDR = (ModuleAddress<<1)+1;
		TWCR = _BV(TWINT)|_BV(TWEN);
		loop_until_bit_is_set(TWCR, TWINT);
		//twi read
		TWCR = _BV(TWINT)|_BV(TWEN);
		loop_until_bit_is_set(TWCR, TWINT);
		printf("Data: %x\r\n", TWDR);
		//twi stop
		TWCR = _BV(TWINT)|_BV(TWSTO)|_BV(TWEN);
		_delay_ms(1000);
	}
	return 0;
}

