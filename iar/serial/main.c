#include <iom328p.h>
#include <intrinsics.h>

int putchar(int ch)
{
	while(!UCSR0A_UDRE0);
	UDR0 = ch;
        return ch;
}

#include <stdio.h>

int main(void)
{
        UBRR0L = (((8000000/(9600*16UL)))-1);
        UCSR0C_UCSZ00 = 1;
        UCSR0C_UCSZ01 = 1;
        UCSR0B_RXEN0 = 1;
        UCSR0B_TXEN0 = 1;
	printf("Hello avr World!\n\r");

        while(1)
        {
            //TODO:: Please write your application code
        }

	return 0;
}

