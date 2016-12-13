#include <asf.h>
#include "interrupt.h"
#include "setup.h"

void ISR_init(void)
{
	PORTC.INT0MASK = PIN1_bm;
	PORTC.INTCTRL = PORT_INT0LVL_HI_gc;
}

ISR(PORTC_INT0_vect)
{
	ad7705_callback();
}