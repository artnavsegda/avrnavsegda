#line 1 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
#line 1 "c:/users/art navsegda/coding/avrnavsegda/http_demo/__ethenc28j60.h"
#line 119 "c:/users/art navsegda/coding/avrnavsegda/http_demo/__ethenc28j60.h"
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
#line 146 "c:/users/art navsegda/coding/avrnavsegda/http_demo/__ethenc28j60.h"
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
#line 29 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
sfr sbit SPI_Ethernet_Rst at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_CS at PORTC_OUT.B1;
sfr sbit SPI_Ethernet_Rst_Direction at PORTC_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction at PORTC_DIR.B1;
#line 38 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
const code unsigned char httpHeader[] = "HTTP/1.1 200 OK\nContent-type: " ;
const code unsigned char httpMimeTypeHTML[] = "text/html\n\n" ;
const code unsigned char httpMimeTypeScript[] = "text/plain\n\n" ;
unsigned char httpMethod[] = "GET /";
#line 48 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
const code char *indexPage =
#line 66 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
"<meta http-equiv=\"refresh\" content=\"3;url=http://192.168.20.60\"><HTML><HEAD></HEAD><BODY><h1>AVR + ENC28J60 Mini Web Server</h1><a href=/>Reload</a><script src=/s></script><table><tr><td><table border=1 style=\"font-size:20px ;font-family: terminal ;\"><tr><th colspan=2>PINC</th></tr><script>var str,i;str=\"\";for(i=0;i<8;i++){str+=\"<tr><td bgcolor=pink>BUTTON #\"+i+\"</td>\";if(PINC&(1<<i)){str+=\"<td bgcolor=red>ON\";}else {str+=\"<td bgcolor=#cccccc>OFF\";}str+=\"</td></tr>\";}document.write(str) ;</script>" ;
#line 83 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
const char *indexPage2 = "</table></td><td><table border=1 style=\"font-size:20px ;font-family: terminal ;\"><tr><th colspan=3>PORTD</th></tr><script>var str,i;str=\"\";for(i=0;i<8;i++){str+=\"<tr><td bgcolor=yellow>LED #\"+i+\"</td>\";if(PORTD&(1<<i)){str+=\"<td bgcolor=red>ON\";}else {str+=\"<td bgcolor=#cccccc>OFF\";}str+=\"</td><td><a href=/t\"+i+\">Toggle</a></td></tr>\";}document.write(str) ;</script></table></td></tr></table>This is HTTP request #<script>document.write(REQ)</script></BODY></HTML>" ;
#line 88 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
unsigned char myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;
unsigned char myIpAddr[4] = {192, 168, 1, 150} ;
unsigned char gwIpAddr[4] = {192, 168, 1, 1 } ;
unsigned char ipMask[4] = {255, 255, 255, 0 } ;
unsigned char dnsIpAddr[4] = {192, 168, 1, 1 } ;

unsigned char getRequest[15] ;
unsigned char dyna[29] ;
unsigned long httpCounter = 0 ;
#line 161 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
unsigned int SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
 {
 unsigned int len;






 if(localPort != 80)
 {
 return(0) ;
 }


 for(len = 0 ; len < 10 ; len++)
 {
 getRequest[len] = SPI_Ethernet_getByte() ;
 }
 getRequest[len] = 0 ;

 len = 0;

 if(memcmp(getRequest, httpMethod, 5))
 {
 return(0) ;
 }

 httpCounter++ ;

 if(getRequest[5] == 's')
 {



 len =  SPI_Ethernet_putConstString (httpHeader) ;
 len +=  SPI_Ethernet_putConstString (httpMimeTypeScript) ;


 len +=  SPI_Ethernet_putConstString ("var PINC=") ;
 WordToStr(PORTC_IN, dyna) ;
 len +=  SPI_Ethernet_putString (dyna) ;
 len +=  SPI_Ethernet_putConstString (";") ;


 len +=  SPI_Ethernet_putConstString ("var PORTD=") ;
 WordToStr(PORTD_OUT, dyna) ;
 len +=  SPI_Ethernet_putString (dyna) ;
 len +=  SPI_Ethernet_putConstString (";") ;


 WordToStr(httpCounter, dyna) ;
 len +=  SPI_Ethernet_putConstString ("var REQ=") ;
 len +=  SPI_Ethernet_putString (dyna) ;
 len +=  SPI_Ethernet_putConstString (";") ;
 }
 else if(getRequest[5] == 't')
 {
 unsigned char bitMask = 0 ;

 if(isdigit(getRequest[6]))
 {
 bitMask = getRequest[6] - '0' ;
 bitMask = 1 << bitMask ;
 PORTD_OUT ^= bitMask ;
 }
 }

 if(len == 0)
 {
 len =  SPI_Ethernet_putConstString (httpHeader) ;
 len +=  SPI_Ethernet_putConstString (httpMimeTypeHTML) ;
 len +=  SPI_Ethernet_putConstString (indexPage) ;
 len +=  SPI_Ethernet_putConstString (indexPage2) ;
 }

 return(len) ;
 }
#line 250 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
unsigned int SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
 {
 unsigned int len ;


 ByteToStr(remoteHost[0], dyna) ;
 dyna[3] = '.' ;
 ByteToStr(remoteHost[1], dyna + 4) ;
 dyna[7] = '.' ;
 ByteToStr(remoteHost[2], dyna + 8) ;
 dyna[11] = '.' ;
 ByteToStr(remoteHost[3], dyna + 12) ;

 dyna[15] = ':' ;


 WordToStr(remotePort, dyna + 16) ;
 dyna[21] = '[' ;
 WordToStr(destPort, dyna + 22) ;
 dyna[27] = ']' ;
 dyna[28] = 0 ;


 len = 28 + reqLength;


 SPI_Ethernet_putBytes(dyna, 28) ;


 while(reqLength--)
 {
 SPI_Ethernet_putByte(toupper(SPI_Ethernet_getByte())) ;
 }

 return(len) ;
 }
#line 289 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
void main()
 {
 OSC_CTRL = 0x02;

 while(RC32MRDY_bit == 0)
 ;

 CPU_CCP = 0xD8;
 CLK_CTRL = 1;
#line 310 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
 PORTC_DIR.B7 = 1;
 PORTC_DIR.B5 = 1;
 PORTC_DIR.B6 = 0;
 PORTB_DIR.B6 = 0;
 PORTB_OUT.B6 = 0;

 PORTC_DIR.B0 = 1;
 PORTC_DIR.B1 = 1;

 SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);

 SPI_Set_Active(&SPIC_Read,&SPIC_Write);
 Spi_Rd_Ptr = SPIC_Read;
 SPI_Ethernet_Init(myMacAddr, myIpAddr,  0x01 ) ;


 SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr) ;

 while(1)
 {
#line 333 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
 SPI_Ethernet_doPacket() ;
#line 340 "C:/Users/Art Navsegda/Coding/avrnavsegda/http_demo/HTTP_Demo.c"
 }
 }
