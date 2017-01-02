#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/prototype/setup.c"
#line 1 "c:/users/artna/documents/github/avrnavsegda/prototype/prototype.h"

void PrintHandler(char c);
void main();
#line 1 "c:/users/artna/documents/github/avrnavsegda/prototype/settings.h"
#line 1 "c:/users/artna/documents/github/avrnavsegda/prototype/setup.h"
void Sysclk_Init(void);
void IOPort_Init();
void Prototype_Init();
#line 5 "C:/Users/artna/Documents/GitHub/avrnavsegda/prototype/setup.c"
sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction at PORTC_DIR.B0;

unsigned char myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;
unsigned char myIpAddr[4] = {192, 168, 1, 150} ;
unsigned char gwIpAddr[4] = {192, 168, 1, 1 } ;
unsigned char ipMask[4] = {255, 255, 255, 0 } ;
unsigned char dnsIpAddr[4] = {192, 168, 1, 1 } ;

void Sysclk_Init(void)
{
 OSC_CTRL = 0x02;
 while (RC32MRDY_bit == 0);
 CPU_CCP = 0xD8;
 CLK_CTRL = 1;
}

void IOPort_Init(void)
{
 PORTC_DIR.B7 = 1;
 PORTC_DIR.B5 = 1;
 PORTC_DIR.B6 = 0;
 PORTB_DIR.B6 = 0;
 PORTB_OUT.B6 = 0;
 PORTC_DIR.B0 = 1;
 PORTC_DIR.B1 = 1;
 PORTD_DIRSET = 0xFF;
 PORTD_OUTCLR = 0xFF;
}

void Interrupt_Init(void)
{
 PMIC_CTRL = 4;
 CPU_SREG.B7 = 1;
}

void Prototype_Init(void)
{

 Sysclk_Init();


 IOPort_Init();


 UARTC0_Init(115200);
 UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);


 SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
 SPI_Set_Active(&SPIC_Read,&SPIC_Write);
 Spi_Rd_Ptr = SPIC_Read;


 SPI_Ethernet_Init(myMacAddr, myIpAddr, 1);
 SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr);


 Interrupt_Init();


 Timer_Init(&TCC0, 1000000);
 Timer_Interrupt_Enable(&TCC0);
}
