#include <asf.h>
#include "interrupt.h"

void ISR_init(void)
{
	PORTC.INT0MASK = PIN1_bm;
	PORTE.INT0MASK = PIN5_bm;
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INT1MASK = PIN2_bm;
	PORTC.INTCTRL = PORT_INT0LVL_HI_gc;
	PORTE.INTCTRL = PORT_INT0LVL_LO_gc;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
}

ISR(PORTC_INT0_vect)
{
	ad7705_callback();
}
