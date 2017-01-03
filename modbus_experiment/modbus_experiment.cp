#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/modbus_experiment/modbus_experiment.c"
#line 1 "c:/users/artna/documents/github/avrnavsegda/modbus_experiment/__ethenc28j60.h"
#line 119 "c:/users/artna/documents/github/avrnavsegda/modbus_experiment/__ethenc28j60.h"
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
#line 146 "c:/users/artna/documents/github/avrnavsegda/modbus_experiment/__ethenc28j60.h"
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
#line 7 "C:/Users/artna/Documents/GitHub/avrnavsegda/modbus_experiment/modbus_experiment.c"
sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction at PORTC_DIR.B0;


sbit ad7705_Chip_Select at PORTC_OUT.B4;
sbit ad7705_Chip_Select_Direction at PORTC_DIR.B4;
sbit ad7707_drdy at PORTC_IN.B1;
#line 20 "C:/Users/artna/Documents/GitHub/avrnavsegda/modbus_experiment/modbus_experiment.c"
unsigned char myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;
unsigned char myIpAddr[4] = {192, 168, 1, 150} ;
unsigned char gwIpAddr[4] = {192, 168, 1, 1 } ;
unsigned char ipMask[4] = {255, 255, 255, 0 } ;
unsigned char dnsIpAddr[4] = {192, 168, 1, 1 } ;
#line 33 "C:/Users/artna/Documents/GitHub/avrnavsegda/modbus_experiment/modbus_experiment.c"
void PrintHandler(char c)
{
 UART_Write(c);
}

struct askreadregstruct {
 unsigned int firstreg;
 unsigned int regnumber;
};

struct reqreadcoilsstruct {
 unsigned char bytestofollow;
 unsigned char coils[256];
};

struct reqreadwordstruct {
 unsigned char bytestofollow;
 unsigned int registers[127];
};

struct writeregstruct {
 unsigned int regaddress;
 unsigned int regvalue;
};

union pdudataunion {
 struct askreadregstruct askreadregs;
 struct reqreadcoilsstruct reqreadcoils;
 struct reqreadwordstruct reqreadholdings;
 struct writeregstruct writereg;
 unsigned int words[128];
 unsigned char bytes[256];
};

struct pduframestruct {
 unsigned char unitid;
 unsigned char fncode;
 union pdudataunion values;
};

struct mbframestruct {
 unsigned int tsid;
 unsigned int protoid;
 unsigned int length;
 struct pduframestruct pdu;
};

struct mbframestruct askframe;



unsigned int table[100] = {0xABCD, 0xDEAD, 0x0000};
unsigned int amount = 100;

unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
 int i;
 unsigned int len;
 int firstrequest = 0;
 int requestnumber = 0;






 if(localPort != 502)
 {
 return(0) ;
 }

 PrintOut(PrintHandler, "Recieved %d bytes\r\n", reqLength);

 for(i = 0;i < reqLength; i++)
 {

 ((char *)&askframe)[i] = SPI_Ethernet_getByte();



 PrintOut(PrintHandler, "%#02x ", (unsigned int)((char *)&askframe)[i]);

 }
 UART_Write_Text("\r\n");

 PrintOut(PrintHandler, "TS id: %u\r\n",  ((((askframe.tsid) >> 8) & 0xff) | (((askframe.tsid) & 0xff) << 8)) );
 PrintOut(PrintHandler, "Protocol id: %u\r\n",  ((((askframe.protoid) >> 8) & 0xff) | (((askframe.protoid) & 0xff) << 8)) );
 PrintOut(PrintHandler, "Length PDU unit: %u\r\n",  ((((askframe.length) >> 8) & 0xff) | (((askframe.length) & 0xff) << 8)) );

 PrintOut(PrintHandler, "Unit id: %u\r\n", (unsigned int)askframe.pdu.unitid);
 PrintOut(PrintHandler, "Function code: %u\r\n", (unsigned int)askframe.pdu.fncode);

 switch (askframe.pdu.fncode)
 {
 case 1:
 case 2:
 PrintOut(PrintHandler, "Number of C/D registers requested: %d\r\n",  ((((askframe.pdu.values.askreadregs.regnumber) >> 8) & 0xff) | (((askframe.pdu.values.askreadregs.regnumber) & 0xff) << 8)) );
 askframe.pdu.values.reqreadcoils.bytestofollow =  ((((askframe.pdu.values.askreadregs.regnumber) >> 8) & 0xff) | (((askframe.pdu.values.askreadregs.regnumber) & 0xff) << 8))  / 8;
 if (( ((((askframe.pdu.values.askreadregs.regnumber) >> 8) & 0xff) | (((askframe.pdu.values.askreadregs.regnumber) & 0xff) << 8))  % 8)>0)
 askframe.pdu.values.reqreadcoils.bytestofollow++;
 askframe.length =  ((((askframe.pdu.values.reqreadcoils.bytestofollow + 3) >> 8) & 0xff) | (((askframe.pdu.values.reqreadcoils.bytestofollow + 3) & 0xff) << 8)) ;

 for (i = 0; i < askframe.pdu.values.reqreadcoils.bytestofollow; i++)
 askframe.pdu.values.reqreadcoils.coils[i] = 0x00;
 break;
 case 3:
 case 4:
 firstrequest =  ((((askframe.pdu.values.askreadregs.firstreg) >> 8) & 0xff) | (((askframe.pdu.values.askreadregs.firstreg) & 0xff) << 8)) ;
 PrintOut(PrintHandler, "Requesing register starting from:: %d\r\n", firstrequest);
 requestnumber =  ((((askframe.pdu.values.askreadregs.regnumber) >> 8) & 0xff) | (((askframe.pdu.values.askreadregs.regnumber) & 0xff) << 8)) ;
 PrintOut(PrintHandler, "Numer of H/I registers requested: %d\r\n", requestnumber);
 askframe.pdu.values.reqreadholdings.bytestofollow = requestnumber * 2;
 askframe.length =  ((((askframe.pdu.values.reqreadholdings.bytestofollow + 3) >> 8) & 0xff) | (((askframe.pdu.values.reqreadholdings.bytestofollow + 3) & 0xff) << 8)) ;

 for (i = 0; i < requestnumber;i++)
 {
 if(firstrequest+i < amount)
 askframe.pdu.values.reqreadholdings.registers[i] =  ((((table[firstrequest+i]) >> 8) & 0xff) | (((table[firstrequest+i]) & 0xff) << 8)) ;
 else
 askframe.pdu.values.reqreadholdings.registers[i] =  ((((0x0000) >> 8) & 0xff) | (((0x0000) & 0xff) << 8)) ;
 }
 break;
 case 5:
 case 6:

 break;
 case 15:
 case 16:
 askframe.length =  ((((6) >> 8) & 0xff) | (((6) & 0xff) << 8)) ;
 default:
 break;
 }

 len =  ((((askframe.length) >> 8) & 0xff) | (((askframe.length) & 0xff) << 8))  + 6;
 PrintOut(PrintHandler, "Reply length: %d\r\n", len);

 SPI_Ethernet_putBytes((unsigned char *)&askframe,len);


 return(len);

}

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
 return(0);
}

int tick;

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF {
 tick = 1;
}

unsigned int result;

void SPIC_Read_Bytes(char *buffer, unsigned NoBytes)
{
 int i;
 for (i = 0; i < NoBytes; i++)
 buffer[i] = SPIC_Read(0xFF);
}

void ad7705_init(void)
{
 SPI_Ethernet_CS = 1;
 ad7705_Chip_Select = 0;

 SPIC_Write(0xFF);
 SPIC_Write(0xFF);
 SPIC_Write(0xFF);
 SPIC_Write(0xFF);
 SPIC_Write(0xFF);

 delay_ms(10);

 SPIC_Write(0x20);
 SPIC_Write(0x0C);
 SPIC_Write(0x10);
 SPIC_Write(0x04);

 delay_ms(10);

 SPIC_Write(0x60);
 SPIC_Write(0x18);
 SPIC_Write(0x3A);
 SPIC_Write(0x00);

 delay_ms(10);

 SPIC_Write(0x70);
 SPIC_Write(0x89);
 SPIC_Write(0x78);
 SPIC_Write(0xD7);
 ad7705_Chip_Select = 1;
 SPI_Ethernet_CS = 0;
}
#line 234 "C:/Users/artna/Documents/GitHub/avrnavsegda/modbus_experiment/modbus_experiment.c"
void main()
{
 int i;
 OSC_CTRL = 0x02;
 while(RC32MRDY_bit == 0)
 ;
 CPU_CCP = 0xD8;
 CLK_CTRL = 1;
#line 250 "C:/Users/artna/Documents/GitHub/avrnavsegda/modbus_experiment/modbus_experiment.c"
 PORTC_DIR.B7 = 1;
 PORTC_DIR.B5 = 1;
 PORTC_DIR.B6 = 0;
 PORTB_DIR.B6 = 0;
 PORTB_OUT.B6 = 0;
 PORTC_DIR.B0 = 1;
 PORTC_DIR.B1 = 0;
 PORTD_DIR.B4 = 1;



 UARTC0_Init(115200);
 UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);

 UART_Write_Text("Starting\r\n");
 PrintOut(PrintHandler, "Testing output\r\n");

 PMIC_CTRL = 4;
 CPU_SREG.B7 = 1;

 Timer_Init(&TCC0, 1000000);
 Timer_Interrupt_Enable(&TCC0);

 SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
 SPI_Set_Active(&SPIC_Read,&SPIC_Write);

 ad7705_init();

 Spi_Rd_Ptr = SPIC_Read;
 SPI_Ethernet_Init(myMacAddr, myIpAddr,  0x01 ) ;


 SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr) ;

 while(1)
 {
#line 289 "C:/Users/artna/Documents/GitHub/avrnavsegda/modbus_experiment/modbus_experiment.c"
 ad7705_Chip_Select = 1;
 SPI_Ethernet_doPacket() ;
 ad7705_Chip_Select = 0;

 if (ad7707_drdy == 0)
 {

 ad7705_Chip_Select = 0;
 SPIC_Write(0x38);
 SPIC_Read_Bytes((unsigned char *)&result,2);
 ad7705_Chip_Select = 1;
 }

 if (tick == 1)
 {
 PORTD_OUTTGL.B4 = 1;
 table[2]++;
 table[3] =  ((((result) >> 8) & 0xff) | (((result) & 0xff) << 8)) ;
 for (i = 0; i<8;i++)
 table[4+i] = ADCA_Read(i);
 for (i = 0; i<8;i++)
 table[12+i] = ADCB_Read(i);
 tick = 0;

 }
 }
}
