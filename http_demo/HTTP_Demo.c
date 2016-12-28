/*
     - mikroETH board is designed to use 3.3V power supply only.
       Be careful to use voltage translators on communication lines when connecting it to 5V systems.
     - Since the ENC28J60 doesn't support auto-negotiation, full-duplex mode is
       not compatible with most switches/routers.  If a dedicated network is used
       where the duplex of the remote node can be manually configured, you may
       change this configuration.  Otherwise, half duplex should always be used.
     - Turn ON PORTD LEDs at SW12 and pull-down PORTC at SW3. (board specific)
     - Connect mikroETH board pins as following:
          mikroETH pin                    ATxmega128A1
              1 (VCC)   <------------->     VCC
              2 (GND)   <------------->     GND
              3 (SO)    <------------->     PC6
              4 (SI)    <------------->     PC5
              5 (SCK)   <------------->     PC7
              6 (GND)   <------------->     NOT CONNECTED
              7 (INT)   <------------->     NOT CONNECTED
              8 (NC)    <------------->     NOT CONNECTED
              9 (RST)   <------------->     PC1
             10 (CS)    <------------->     PC0
 */

#include  "__EthEnc28j60.h"
// duplex config flags
#define Spi_Ethernet_HALFDUPLEX     0x00  // half duplex
#define Spi_Ethernet_FULLDUPLEX     0x01  // full duplex

// mE ehternet NIC pinout
sfr sbit SPI_Ethernet_Rst at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B1;
sfr sbit SPI_Ethernet_Rst_Direction at PORTC_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B1;
// end ethernet NIC definitions

/************************************************************
 * ROM constant strings
 */
const code unsigned char httpHeader[] = "HTTP/1.1 200 OK\nContent-type: " ;  // HTTP header
const code unsigned char httpMimeTypeHTML[] = "text/html\n\n" ;              // HTML MIME type
const code unsigned char httpMimeTypeScript[] = "text/plain\n\n" ;           // TEXT MIME type
unsigned char httpMethod[] = "GET /";
/*
 * web page, splited into 2 parts :
 * when coming short of ROM, fragmented data is handled more efficiently by linker
 *
 * this HTML page calls the boards to get its status, and builds itself with javascript
 */
const code char    *indexPage =                   // Change the IP address of the page to be refreshed
"<meta http-equiv=\"refresh\" content=\"3;url=http://192.168.20.60\">\
<HTML><HEAD></HEAD><BODY>\
<h1>AVR + ENC28J60 Mini Web Server</h1>\
<a href=/>Reload</a>\
<script src=/s></script>\
<table><tr><td><table border=1 style=\"font-size:20px ;font-family: terminal ;\">\
<tr><th colspan=2>PINC</th></tr>\
<script>\
var str,i;\
str=\"\";\
for(i=0;i<8;i++)\
{str+=\"<tr><td bgcolor=pink>BUTTON #\"+i+\"</td>\";\
if(PINC&(1<<i)){str+=\"<td bgcolor=red>ON\";}\
else {str+=\"<td bgcolor=#cccccc>OFF\";}\
str+=\"</td></tr>\";}\
document.write(str) ;\
</script>\
" ;

const   char    *indexPage2 =  "</table></td><td>\
<table border=1 style=\"font-size:20px ;font-family: terminal ;\">\
<tr><th colspan=3>PORTD</th></tr>\
<script>\
var str,i;\
str=\"\";\
for(i=0;i<8;i++)\
{str+=\"<tr><td bgcolor=yellow>LED #\"+i+\"</td>\";\
if(PORTD&(1<<i)){str+=\"<td bgcolor=red>ON\";}\
else {str+=\"<td bgcolor=#cccccc>OFF\";}\
str+=\"</td><td><a href=/t\"+i+\">Toggle</a></td></tr>\";}\
document.write(str) ;\
</script>\
</table></td></tr></table>\
This is HTTP request #<script>document.write(REQ)</script></BODY></HTML>\
" ;

/***********************************
 * RAM variables
 */
unsigned char   myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;   // my MAC address
unsigned char   myIpAddr[4]  = {192, 168, 1, 150} ;                     // my IP address
unsigned char   gwIpAddr[4]  = {192, 168,  1,  1 } ;                   // gateway (router) IP address
unsigned char   ipMask[4]    = {255, 255, 255,  0 } ;                   // network mask (for example : 255.255.255.0)
unsigned char   dnsIpAddr[4] = {192, 168,  1,  1 } ;                   // DNS server IP address

unsigned char   getRequest[15] ;                                        // HTTP request buffer
unsigned char   dyna[29] ;                                              // buffer for dynamic response
unsigned long   httpCounter = 0 ;                                       // counter of HTTP requests

/*******************************************
 * functions
 */

/*
 * put the constant string pointed to by s to the ENC transmit buffer.
 */
/*unsigned int    putConstString(const code char *s)
        {
        unsigned int ctr = 0 ;

        while(*s)
                {
                Spi_Ethernet_putByte(*s++) ;
                ctr++ ;
                }
        return(ctr) ;
        }*/
/*
 * it will be much faster to use library Spi_Ethernet_putConstString routine
 * instead of putConstString routine above. However, the code will be a little
 * bit bigger. User should choose between size and speed and pick the implementation that
 * suites him best. If you choose to go with the putConstString definition above
 * the #define line below should be commented out.
 *
 */
#define putConstString  SPI_Ethernet_putConstString

/*
 * put the string pointed to by s to the ENC transmit buffer
 */
/*unsigned int    putString(char *s)
        {
        unsigned int ctr = 0 ;

        while(*s)
                {
                Spi_Ethernet_putByte(*s++) ;

                ctr++ ;
                }
        return(ctr) ;
        }*/
/*
 * it will be much faster to use library Spi_Ethernet_putString routine
 * instead of putString routine above. However, the code will be a little
 * bit bigger. User should choose between size and speed and pick the implementation that
 * suites him best. If you choose to go with the putString definition above
 * the #define line below should be commented out.
 *
 */
#define putString  SPI_Ethernet_putString

/*
 * this function is called by the library
 * the user accesses to the HTTP request by successive calls to Spi_Ethernet_getByte()
 * the user puts data in the transmit buffer by successive calls to Spi_Ethernet_putByte()
 * the function must return the length in bytes of the HTTP reply, or 0 if nothing to transmit
 *
 * if you don't need to reply to HTTP requests,
 * just define this function with a return(0) as single statement
 *
 */
unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
        {
        unsigned int    len;            // my reply length

        // should we close tcp socket after response is sent?
        // library closes tcp socket by default if canClose flag is not reset here
        // *canClose = 0; // 0 - do not close socket
                          // otherwise - close socket

        if(localPort != 80)             // I listen only to web request on port 80
                {
                return(0) ;
                }

        // get 10 first bytes only of the request, the rest does not matter here
        for(len = 0 ; len < 10 ; len++)
                {
                getRequest[len] = SPI_Ethernet_getByte() ;
                }
        getRequest[len] = 0 ;

        len = 0;

        if(memcmp(getRequest, httpMethod, 5))       // only GET method is supported here
                {
                return(0) ;
                }

        httpCounter++ ;                             // one more request done

        if(getRequest[5] == 's')                    // if request path name starts with s, store dynamic data in transmit buffer
                {
                // the text string replied by this request can be interpreted as javascript statements
                // by browsers

                len = putConstString(httpHeader) ;              // HTTP header
                len += putConstString(httpMimeTypeScript) ;     // with text MIME type

                // add PORTC value (buttons) to reply
                len += putConstString("var PINC=") ;
                WordToStr(PORTC_IN, dyna) ;
                len += putString(dyna) ;
                len += putConstString(";") ;

                // add PORTD value (LEDs) to reply
                len += putConstString("var PORTD=") ;
                WordToStr(PORTD_OUT, dyna) ;
                len += putString(dyna) ;
                len += putConstString(";") ;

                // add HTTP requests counter to reply
                WordToStr(httpCounter, dyna) ;
                len += putConstString("var REQ=") ;
                len += putString(dyna) ;
                len += putConstString(";") ;
                }
        else if(getRequest[5] == 't')                           // if request path name starts with t, toggle P3 (LED) bit number that comes after
                {
                unsigned char   bitMask = 0 ;                   // for bit mask

                if(isdigit(getRequest[6]))                      // if 0 <= bit number <= 9, bits 8 & 9 does not exist but does not matter
                        {
                        bitMask = getRequest[6] - '0' ;         // convert ASCII to integer
                        bitMask = 1 << bitMask ;                // create bit mask
                        PORTD_OUT ^= bitMask ;                      // toggle PORTD with xor operator
                        }
                }

        if(len == 0)                                            // what do to by default
                {
                len =  putConstString(httpHeader) ;             // HTTP header
                len += putConstString(httpMimeTypeHTML) ;       // with HTML MIME type
                len += putConstString(indexPage) ;              // HTML page first part
                len += putConstString(indexPage2) ;             // HTML page second part
                }

        return(len) ;                                           // return to the library with the number of bytes to transmit
        }

/*
 * this function is called by the library
 * the user accesses to the UDP request by successive calls to Spi_Ethernet_getByte()
 * the user puts data in the transmit buffer by successive calls to Spi_Ethernet_putByte()
 * the function must return the length in bytes of the UDP reply, or 0 if nothing to transmit
 *
 * if you don't need to reply to UDP requests,
 * just define this function with a return(0) as single statement
 *
 */
unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
        {
        unsigned int    len ;                           // my reply length

        // reply is made of the remote host IP address in human readable format
        ByteToStr(remoteHost[0], dyna) ;                // first IP address byte
        dyna[3] = '.' ;
        ByteToStr(remoteHost[1], dyna + 4) ;            // second
        dyna[7] = '.' ;
        ByteToStr(remoteHost[2], dyna + 8) ;            // third
        dyna[11] = '.' ;
        ByteToStr(remoteHost[3], dyna + 12) ;           // fourth

        dyna[15] = ':' ;                                // add separator

        // then remote host port number
        WordToStr(remotePort, dyna + 16) ;
        dyna[21] = '[' ;
        WordToStr(destPort, dyna + 22) ;
        dyna[27] = ']' ;
        dyna[28] = 0 ;

        // the total length of the request is the length of the dynamic string plus the text of the request
        len = 28 + reqLength;

        // puts the dynamic string into the transmit buffer
        SPI_Ethernet_putBytes(dyna, 28) ;

        // then puts the request string converted into upper char into the transmit buffer
        while(reqLength--)
                {
                SPI_Ethernet_putByte(toupper(SPI_Ethernet_getByte())) ;
                }

        return(len) ;           // back to the library with the length of the UDP reply
        }
/*
 * main entry
 */
void    main()
        {
          OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator

          while(RC32MRDY_bit == 0)
              ;

          CPU_CCP = 0xD8;
          CLK_CTRL = 1;
        // set PORTC as input
        //PORTC_DIR = 0;
        // set PORTD as output
        //PORTD_DIR = 0xFF;

        /*
         * starts ENC28J60 with :
         * reset bit on PB4
         * CS bit on PB5
         * my MAC & IP address
         * full duplex
         */
        PORTC_DIR.B7 = 1;
        PORTC_DIR.B5 = 1;
        PORTC_DIR.B6 = 0;
        PORTB_DIR.B6 = 0;
        PORTB_OUT.B6 = 0;
        
        PORTC_DIR.B0 = 1;
        PORTC_DIR.B1 = 1;
        
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
        //SPIC_Init();
        SPI_Set_Active(&SPIC_Read,&SPIC_Write);
        Spi_Rd_Ptr = SPIC_Read;
        SPI_Ethernet_Init(myMacAddr, myIpAddr, Spi_Ethernet_FULLDUPLEX) ;

        // dhcp will not be used here, so use preconfigured addresses
        SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr) ;

        while(1) // do forever
                {
                /*
                 * if necessary, test the return value to get error code
                 */
                SPI_Ethernet_doPacket() ;   // process incoming Ethernet packets

                /*
                 * add your stuff here if needed
                 * Spi_Ethernet_doPacket() must be called as often as possible
                 * otherwise packets could be lost
                 */
                }
        }