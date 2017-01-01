#include  "__EthEnc28j60.h"
// duplex config flags
#define Spi_Ethernet_HALFDUPLEX     0x00  // half duplex
#define Spi_Ethernet_FULLDUPLEX     0x01  // full duplex

// mE ehternet NIC pinout
sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;
// end ethernet NIC definitions

/***********************************
 * RAM variables
 */
unsigned char   myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;   // my MAC address
unsigned char   myIpAddr[4]  = {192, 168, 1, 150} ;                     // my IP address
unsigned char   gwIpAddr[4]  = {192, 168,  1,  1 } ;                   // gateway (router) IP address
unsigned char   ipMask[4]    = {255, 255, 255,  0 } ;                   // network mask (for example : 255.255.255.0)
unsigned char   dnsIpAddr[4] = {192, 168,  1,  1 } ;                   // DNS server IP address

/*******************************************
 * functions
 */

#define putConstString  SPI_Ethernet_putConstString
#define putString  SPI_Ethernet_putString

void PrintHandler(char c)
{
        UART_Write(c);
}

struct askreadregstruct {
	unsigned short firstreg;
	unsigned short regnumber;
};

struct reqreadcoilsstruct {
	unsigned char bytestofollow;
	unsigned char coils[256];
};

struct reqreadwordstruct {
	unsigned char bytestofollow;
	unsigned short registers[127];
};

struct writeregstruct {
	unsigned short regaddress;
	unsigned short regvalue;
};

union pdudataunion {
	struct askreadregstruct askreadregs;
	struct reqreadcoilsstruct reqreadcoils;
	struct reqreadwordstruct reqreadholdings;
	struct writeregstruct writereg;
	unsigned short words[128];
	unsigned char bytes[256];
};

struct pduframestruct {
        unsigned char unitid;
        unsigned char fncode;
        union pdudataunion values;
};

struct mbframestruct {
        unsigned short tsid;
        unsigned short protoid;
        unsigned short length;
        struct pduframestruct pdu;
};

struct mbframestruct askframe;

unsigned char buf[100];

#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        int i;
        unsigned int len;            // my reply length

        // should we close tcp socket after response is sent?
        // library closes tcp socket by default if canClose flag is not reset here
        // *canClose = 0; // 0 - do not close socket
        // otherwise - close socket

        if(localPort != 502)             // I listen only to web request on port 80
        {
                return(0) ;
        }

        PrintOut(PrintHandler, "Recieved %d bytes\r\n", reqLength);

        for(i = 0;i < reqLength; i++)
        {
                ((char *)&askframe)[i] = SPI_Ethernet_getByte();
                buf[i] = SPI_Ethernet_getByte();
        //        Spi_Ethernet_putByte(buf[i]);//send back all recieved modbus packets
        //        PrintOut(PrintHandler, "%c ", buf[i]); // print ascii chars
                PrintOut(PrintHandler, "%#02x ", (unsigned int)buf[i]); // print hex bytes
        //        PrintOut(PrintHandler, "%u ", (unsigned int)buf[i]); // print unsigned numbers
        }
        UART_Write_Text("\r\n");
        
        PrintOut(PrintHandler, "TS id: %d\r\n", BSWAP_16(askframe.tsid));
        PrintOut(PrintHandler, "Protocol id: %d\r\n", BSWAP_16(askframe.protoid));
        PrintOut(PrintHandler, "Length PDU unit: %d\r\n", BSWAP_16(askframe.length));
        PrintOut(PrintHandler, "Unit id: %d\r\n", BSWAP_16(askframe.pdu.unitid));
        PrintOut(PrintHandler, "Function code: %d\r\n", BSWAP_16(askframe.pdu.fncode));

        switch (askframe.pdu.fncode)
        {
               case 1:
               case 2:
                      PrintOut(PrintHandler, "Number of C/D registers requested: %d\r\n", BSWAP_16(askframe.pdu.values.askreadregs.regnumber));
                      askframe.pdu.values.reqreadcoils.bytestofollow = askframe.pdu.values.askreadregs.regnumber / 8;
                      if ((askframe.pdu.values.askreadregs.regnumber % 8)>0)
                         askframe.pdu.values.reqreadcoils.bytestofollow++;
                      askframe.length = BSWAP_16(askframe.pdu.values.reqreadcoils.bytestofollow + 3);
               break;
               case 3:
               case 4:
                      PrintOut(PrintHandler, "Numer of H/I registers requested: %d\r\n", BSWAP_16(askframe.pdu.values.askreadregs.regnumber));
                      askframe.pdu.values.reqreadholdings.bytestofollow = BSWAP_16(askframe.pdu.values.askreadregs.regnumber) * 2;
                      askframe.length = BSWAP_16(askframe.pdu.values.reqreadholdings.bytestofollow + 3);
               break;
               default:
               break;
        }
        
        len = BSWAP_16(askframe.length) + 16;
        PrintOut(PrintHandler, "Reply length: %d\r\n", len);
 
        //return(reqLength);//send back all recieved bytes
        return(0);
}

unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return(0);
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
        
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        
        UART_Write_Text("Starting\r\n");
        PrintOut(PrintHandler, "Testing output\r\n");
        
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
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