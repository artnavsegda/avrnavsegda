void main()
{
     TWIE_Init(100000);
     TWIE_Start();
     TWIE_Write(0x1a << 1);          // send byte via TWI (device address + W)
     TWIE_Write(0x03);             // send byte (address of location)
     TWIE_Write(0x00);          // send data (data to be written)
     TWIE_Stop();               // issue TWI stop signal
     
     Delay_100ms();
     
     TWIE_Init(100000);
     TWIE_Start();
     TWIE_Write(0x1a << 1);          // send byte via TWI (device address + W)
     TWIE_Write(0x02);             // send byte (address of location)
     TWIE_Write(0x00);          // send data (data to be written)
     TWIE_Stop();               // issue TWI stop signal
     
     while (1)
     {
       Delay_100ms();

       TWIE_Init(100000);
       TWIE_Start();
       TWIE_Write(0x1a << 1);          // send byte via TWI (device address + W)
       TWIE_Write(0x01);             // send byte (address of location)
       TWIE_Write(0x00);          // send data (data to be written)
       TWIE_Stop();               // issue TWI stop signal

       Delay_100ms();

       TWIE_Init(100000);
       TWIE_Start();
       TWIE_Write(0x1a << 1);          // send byte via TWI (device address + W)
       TWIE_Write(0x01);             // send byte (address of location)
       TWIE_Write(0xFF);          // send data (data to be written)
       TWIE_Stop();               // issue TWI stop signal
     }
}