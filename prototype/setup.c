#include "prototype.h"
#include "settings.h"
#include "setup.h"

sfr sbit SPI_Ethernet_Rst at PORTA_OUT.B0;
sfr sbit SPI_Ethernet_CS  at PORTC_OUT.B0;
sfr sbit SPI_Ethernet_Rst_Direction at PORTA_DIR.B0;
sfr sbit SPI_Ethernet_CS_Direction  at PORTC_DIR.B0;

unsigned char   myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f} ;   // my MAC address
unsigned char   myIpAddr[4]  = {192, 168, 1, 150} ;                     // my IP address
unsigned char   gwIpAddr[4]  = {192, 168,  1,  1 } ;                   // gateway (router) IP address
unsigned char   ipMask[4]    = {255, 255, 255,  0 } ;                   // network mask (for example : 255.255.255.0)
unsigned char   dnsIpAddr[4] = {192, 168,  1,  1 } ;                   // DNS server IP address

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
        PORTD_DIR.B4 = 1;
        PORTC_DIR.B4 = 1; // ad7705 cs
}

void Interrupt_Init(void)
{
        PMIC_CTRL = 4;                    // Enable medium level interrupts
        CPU_SREG.B7 = 1;                  // Enable global interrupts
}

void Prototype_Init(void)
{
        // system clock init
        Sysclk_Init();
        
        // i/o ports init
        IOPort_Init();
        
        // uart init
        UARTC0_Init(115200);
        UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
        
        // i2c init
        TWIE_Init(100000);
        
        // spi init
        SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
        SPI_Set_Active(&SPIC_Read,&SPIC_Write);
        Spi_Rd_Ptr = SPIC_Read;
        
        // ethernet init
        SPI_Ethernet_Init(myMacAddr, myIpAddr, 1);
        SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr);
        
        // interrupt init
        Interrupt_Init();
        
        // timer init
        Timer_Init(&TCC0, 1000000);
        Timer_Interrupt_Enable(&TCC0);
}