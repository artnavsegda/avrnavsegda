#include <avr/io.h>
#include <util/delay.h>
#include "avr_compiler.h"
#include "clksys_driver.h"

int main(void)
{
	CLKSYS_Enable( OSC_RC32MEN_bm );
	while (!CLKSYS_IsReady(OSC_RC32MRDY_bm ));
	CLKSYS_Main_ClockSource_Select(CLK_SCLKSEL_RC32M_gc);
	CLKSYS_Disable(OSC_RC2MEN_bm);

    PORTD_DIRSET = _BV(4);

	while(1)
	{
		PORTD_OUTTGL = _BV(4);
		_delay_ms(1000);
	}
}
