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


unsigned char buf[100];

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        int i;
        unsigned int    len;            // my reply length

        // should we close tcp socket after response is sent?
        // library closes tcp socket by default if canClose flag is not reset here
        // *canClose = 0; // 0 - do not close socket
        // otherwise - close socket

        if(localPort != 502)             // I listen only to web request on port 80
        {
                return(0) ;
        }

        SPI_Ethernet_getBytes(buf, ERDPT, reqLength);

        for(i = 0;i < reqLength; i++)
        {
        //sprintf("0x%02X ",buf[i];
                PrintOut(PrintHandler, "%#2X ");
        }
        // sprintf("\n\r");
        UART_Write_Text("\n\r");
 
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
        
        UART_Write_Text("Starting\n\r");
        
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