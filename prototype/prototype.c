#include "prototype.h"
#include "setup.h"

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
        }
}