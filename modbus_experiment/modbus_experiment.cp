#line 1 "C:/Users/Art Navsegda/Coding/avrnavsegda/modbus_experiment/modbus_experiment.c"
#line 1 "c:/users/art navsegda/coding/avrnavsegda/modbus_experiment/__ethenc28j60.h"
#line 119 "c:/users/art navsegda/coding/avrnavsegda/modbus_experiment/__ethenc28j60.h"
typedef struct
 {
 unsigned char valid;
 unsigned long tmr;
 unsigned char ip[4];
 unsigned char mac[6];
 } SPI_Ethernet_arpCacheStruct;

extern SPI_Ethernet_arpCacheStruct SPI_Ethernet_arpCache[];

extern unsigned char SPI_Ethernet_macAddr[6];
extern unsigned char SPI_Ethernet_ipAddr[4];
extern unsigned char SPI_Ethernet_gwIpAddr[4];
extern unsigned char SPI_Ethernet_ipMask[4];
extern unsigned char SPI_Ethernet_dnsIpAddr[4];
extern unsigned char SPI_Ethernet_rmtIpAddr[4];

extern unsigned long SPI_Ethernet_userTimerSec;

typedef struct {
 unsigned canCloseTCP: 1;
 unsigned isBroadcast: 1;
} TEthPktFlags;
#line 146 "c:/users/art navsegda/coding/avrnavsegda/modbus_experiment/__ethenc28j60.h"
extern void SPI_Ethernet_Init(unsigned char *resetPort, unsigned char resetBit, unsigned char *CSport, unsigned char CSbit, unsigned char *mac, unsigned char *ip, unsigned char fullDuplex);
extern unsigned char SPI_Ethernet_doPacket();
extern void SPI_Ethernet_putByte(unsigned char b);
extern void SPI_Ethernet_putBytes(unsigned char *ptr, unsigned int n);
extern void SPI_Ethernet_putConstBytes(const unsigned char *ptr, unsigned int n);
extern unsigned char SPI_Ethernet_getByte();
extern void SPI_Ethernet_getBytes(unsigned char *ptr, unsigned int addr, unsigned int n);
extern unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags * flags);
extern unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags * flags);
extern void SPI_Ethernet_confNetwork(char *ipMask, char *gwIpAddr, char *dnsIpAddr);
#line 7 "C:/Users/Art Navsegda/Coding/avrnavsegda/modbus_experiment/modbus_experiment.c"
sfr sbit SPI_Ethernet_Rst at PORTC_OUT.B1;
sfr sbit SPI_Ethernet_CS at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTC_DIR.B1;
sfr sbit SPI_Ethernet_CS_Direction at PORTC_DIR.B0;
#line 16 "C:/Users/Art Navsegda/Coding/avrnavsegda/modbus_experiment/modbus_experiment.c"
unsigned char myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;
unsigned char myIpAddr[4] = {192, 168, 1, 150} ;
unsigned char gwIpAddr[4] = {192, 168, 1, 1 } ;
unsigned char ipMask[4] = {255, 255, 255, 0 } ;
unsigned char dnsIpAddr[4] = {192, 168, 1, 1 } ;
#line 29 "C:/Users/Art Navsegda/Coding/avrnavsegda/modbus_experiment/modbus_experiment.c"
void PrintHandler(char c)
{
 UART_Write(c);
}


unsigned char buf[100];

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
 int i;
 unsigned int len;






 if(localPort != 502)
 {
 return(0) ;
 }



 for(i = 0;i < reqLength; i++)
 {
 buf[i] = SPI_Ethernet_getByte();
 Spi_Ethernet_putByte(buf[i]);

 PrintOut(PrintHandler, "%#02x ", (unsigned int)buf[i]);

 }

 UART_Write_Text("\r\n");



 return(reqLength);

}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
 return(0);
}
#line 79 "C:/Users/Art Navsegda/Coding/avrnavsegda/modbus_experiment/modbus_experiment.c"
void main()
{
 OSC_CTRL = 0x02;
 while(RC32MRDY_bit == 0)
 ;
 CPU_CCP = 0xD8;
 CLK_CTRL = 1;
#line 94 "C:/Users/Art Navsegda/Coding/avrnavsegda/modbus_experiment/modbus_experiment.c"
 PORTC_DIR.B7 = 1;
 PORTC_DIR.B5 = 1;
 PORTC_DIR.B6 = 0;
 PORTB_DIR.B6 = 0;
 PORTB_OUT.B6 = 0;
 PORTC_DIR.B0 = 1;
 PORTC_DIR.B1 = 1;

 UARTC0_Init(115200);
 UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);

 UART_Write_Text("Starting\r\n");
 PrintOut(PrintHandler, "Testing output\r\n");

 SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
 SPI_Set_Active(&SPIC_Read,&SPIC_Write);
 Spi_Rd_Ptr = SPIC_Read;
 SPI_Ethernet_Init(myMacAddr, myIpAddr,  0x01 ) ;


 SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr) ;

 while(1)
 {
#line 121 "C:/Users/Art Navsegda/Coding/avrnavsegda/modbus_experiment/modbus_experiment.c"
 SPI_Ethernet_doPacket() ;
#line 128 "C:/Users/Art Navsegda/Coding/avrnavsegda/modbus_experiment/modbus_experiment.c"
 }
}
