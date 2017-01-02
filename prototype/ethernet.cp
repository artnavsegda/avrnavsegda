#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/prototype/ethernet.c"
#line 1 "c:/users/artna/documents/github/avrnavsegda/prototype/prototype.h"

void PrintHandler(char c);
void main();
#line 1 "c:/users/artna/documents/github/avrnavsegda/prototype/__ethenc28j60.h"
#line 119 "c:/users/artna/documents/github/avrnavsegda/prototype/__ethenc28j60.h"
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
#line 146 "c:/users/artna/documents/github/avrnavsegda/prototype/__ethenc28j60.h"
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
#line 1 "c:/users/artna/documents/github/avrnavsegda/prototype/modbus.h"
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
#line 5 "C:/Users/artna/Documents/GitHub/avrnavsegda/prototype/ethernet.c"
struct mbframestruct askframe;
unsigned int table[100] = {0xABCD, 0xDEAD};
unsigned int amount = 100;

unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
 return(0);
}

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
