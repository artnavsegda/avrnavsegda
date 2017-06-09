#include <ioavr.h>
#include <intrinsics.h>
#include <yfuns.h>
#include <stdio.h>

int MyLowLevelPutchar(int x)
{
  while (!UCSR0A_UDRE0);
  UDR0 = x;
  return x;
}

int main(void)
{
        UBRR0L = 51;
        UBRR0H = 0;
        UCSR0C = 6;
        UCSR0B = 24;
        UCSR0A = 0;

        while(1)
        {
            printf("shit");
            //MyLowLevelPutchar('o');
            //__write(_LLIO_STDOUT,"hello",5);
        }

	return 0;
}

