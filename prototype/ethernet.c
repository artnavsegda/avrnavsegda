#include "prototype.h"
#include "__EthEnc28j60.h"
#include "modbus.h"

struct mbframestruct askframe;
unsigned int table[100] = {0xABCD, 0xDEAD};
unsigned int amount = 100;

unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
{
        return(0);
}

unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
{
        int i;
        unsigned int len;            // my reply length
        int firstrequest = 0;
        int requestnumber = 0;

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
        //        buf[i] = SPI_Ethernet_getByte();
                ((char *)&askframe)[i] = SPI_Ethernet_getByte();
        //        Spi_Ethernet_putByte(buf[i]);//send back all recieved modbus packets
        //        PrintOut(PrintHandler, "%c ", buf[i]); // print ascii chars
        //        PrintOut(PrintHandler, "%#02x ", (unsigned int)buf[i]); // print hex bytes
                PrintOut(PrintHandler, "%#02x ", (unsigned int)((char *)&askframe)[i]); // print hex bytes
        //        PrintOut(PrintHandler, "%u ", (unsigned int)buf[i]); // print unsigned numbers
        }
        UART_Write_Text("\r\n");

        PrintOut(PrintHandler, "TS id: %u\r\n", BSWAP_16(askframe.tsid));
        PrintOut(PrintHandler, "Protocol id: %u\r\n", BSWAP_16(askframe.protoid));
        PrintOut(PrintHandler, "Length PDU unit: %u\r\n", BSWAP_16(askframe.length));

        PrintOut(PrintHandler, "Unit id: %u\r\n", (unsigned int)askframe.pdu.unitid);
        PrintOut(PrintHandler, "Function code: %u\r\n", (unsigned int)askframe.pdu.fncode);

        switch (askframe.pdu.fncode)
        {
               case 1:
               case 2:
                      PrintOut(PrintHandler, "Number of C/D registers requested: %d\r\n", BSWAP_16(askframe.pdu.values.askreadregs.regnumber));
                      askframe.pdu.values.reqreadcoils.bytestofollow = BSWAP_16(askframe.pdu.values.askreadregs.regnumber) / 8;
                      if ((BSWAP_16(askframe.pdu.values.askreadregs.regnumber) % 8)>0)
                         askframe.pdu.values.reqreadcoils.bytestofollow++;
                      askframe.length = BSWAP_16(askframe.pdu.values.reqreadcoils.bytestofollow + 3);
                      // fill all requested coil bytes with zeroes
                      for (i = 0; i < askframe.pdu.values.reqreadcoils.bytestofollow; i++)
                          askframe.pdu.values.reqreadcoils.coils[i] = 0x00;
               break;
               case 3:
               case 4:
                      firstrequest = BSWAP_16(askframe.pdu.values.askreadregs.firstreg);
                      PrintOut(PrintHandler, "Requesing register starting from:: %d\r\n", firstrequest);
                      requestnumber = BSWAP_16(askframe.pdu.values.askreadregs.regnumber);
                      PrintOut(PrintHandler, "Numer of H/I registers requested: %d\r\n", requestnumber);
                      askframe.pdu.values.reqreadholdings.bytestofollow = requestnumber * 2;
                      askframe.length = BSWAP_16(askframe.pdu.values.reqreadholdings.bytestofollow + 3);
                      // fill every requested register with 0xABCD
                      for (i = 0; i < requestnumber;i++)
                      {
                          if(firstrequest+i < amount) // if requested register within allocated range
                              askframe.pdu.values.reqreadholdings.registers[i] = BSWAP_16(table[firstrequest+i]);
                          else
                              askframe.pdu.values.reqreadholdings.registers[i] = BSWAP_16(0x0000); // fill up with zeroes
                      }
               break;
               case 5:
               case 6:
                      //same as request
               break;
               case 15:
               case 16:
                      askframe.length = BSWAP_16(6);
               default:
               break;
        }

        len = BSWAP_16(askframe.length) + 6;
        PrintOut(PrintHandler, "Reply length: %d\r\n", len);

        SPI_Ethernet_putBytes((unsigned char *)&askframe,len);

        //return(reqLength);//send back all recieved bytes
        return(len);
        //return(0);
}