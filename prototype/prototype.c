#include "prototype.h"
#include "setup.h"

extern unsigned int table[100];
extern int tick;

void PrintHandler(char c)
{
        UART_Write(c);
}

void main()
{
        Prototype_Init();
        UART_Write_Text("Starting\r\n");
        PrintOut(PrintHandler, "Testing output\r\n");
        
        while(1)
        {
                SPI_Ethernet_doPacket() ;   // process incoming Ethernet packets
		if (tick == 1)
                {
                        PORTD_OUTTGL.B4 = 1;
                        table[2]++;
                        tick = 0;
                }
        }
}