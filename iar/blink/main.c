#include <iom328p.h>
#include <intrinsics.h>

int main(void)
{
	DDRB = 0xFF;
	while (1)
	{
		PORTB = 0xFF;
		__delay_cycles(100000);
		PORTB = 0x00;
		__delay_cycles(100000);
	}
	return 0;
}

