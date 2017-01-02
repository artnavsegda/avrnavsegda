void TWIE_Send(unsigned char devaddr, unsigned char regaddr, unsigned char value)
{
        TWIE_Start();
        TWIE_Write(devaddr << 1);
        TWIE_Write(regaddr);
        TWIE_Write(value);
        TWIE_Stop();
}
