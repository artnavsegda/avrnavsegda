#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/prototype/prototype.c"
#line 1 "c:/users/artna/documents/github/avrnavsegda/prototype/prototype.h"

void PrintHandler(char c);
void main();
#line 1 "c:/users/artna/documents/github/avrnavsegda/prototype/setup.h"
void Sysclk_Init(void);
void IOPort_Init();
void Prototype_Init();
#line 4 "C:/Users/artna/Documents/GitHub/avrnavsegda/prototype/prototype.c"
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
 SPI_Ethernet_doPacket() ;
 }
}
