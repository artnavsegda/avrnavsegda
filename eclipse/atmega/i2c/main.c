#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
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
		TWDR = 0x34;
		TWCR = _BV(TWINT)|_BV(TWEN);
		loop_until_bit_is_set(TWCR, TWINT);
		//twi stop
		TWCR = _BV(TWINT)|_BV(TWSTO)|_BV(TWEN);
		_delay_ms(1000);
	}
	return 0;
}

