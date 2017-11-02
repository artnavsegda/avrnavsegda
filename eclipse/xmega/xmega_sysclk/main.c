#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    OSC_CTRL = 0x02;
    while(OSC_STATUS != OSC_RC32MRDY_bm);
    CPU_CCP = 0xD8;
    CLK_CTRL = 1;

	while(1);
}
