#include <avr/io.h>
#define F_CPU 2000000
#define BAUD 9600
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdio.h>

#include <avr/interrupt.h>


short positive[3];
short negative[3];

short adc(unsigned char controlbyte);

ISR(INT0_vect)
{
	if (bit_is_set(PINB,PB2))
	{
		PORTC &= ~_BV(PC0);
		positive[0] = adc(0x49);
		positive[1] = adc(0x4a);
		positive[2] = adc(0x4b);
	}
	if (bit_is_clear(PINB,PB2))
	{
		PORTC |= _BV(PC0);
		negative[0] = adc(0x49);
		negative[1] = adc(0x4a);
		negative[2] = adc(0x4b);
	}
}

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void write(unsigned char data)
{
	DDRA = 0xFF; // AD line as output

	PORTA = data;
	PORTD &= ~_BV(PD6); // WR line strobe low start
	_delay_us(1);
	PORTD |= _BV(PD6); // WR line strobe low end
}

unsigned char read(void)
{
	unsigned char x;

	DDRA = 0x00; // tristate AD line for data read
	PORTD &= ~_BV(PD7); // RD line strobe low start
	_delay_us(1);
	x = PINA;
	PORTD |= _BV(PD7); // RD line strobe low end
	return x;
}

short adc(unsigned char controlbyte)
{
	short z;
	write(controlbyte);
	//_delay_ms(1);
	loop_until_bit_is_clear(PINB, PB0);
	PORTB &= ~_BV(PD5);
	((char *)&z)[0] = read();
	PORTB |= _BV(PD5);
	((char *)&z)[1] = read();
	return z;
}

int main(void)
{
	DDRB |= _BV(PB3)|_BV(PB4)|_BV(PB5); // gpio latch, mac cs, max hben as outputs
	DDRD |= _BV(PD6)|_BV(PD7); // WR/RD output
	PORTD |= _BV(PD6)|_BV(PD7); // WR/RD idle high	

	DDRC |= _BV(PC0); //PC0 out;

	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;

#if USE_2X
	UCSRA |= _BV(U2X);
#else
	UCSRA &= ~(_BV(U2X));
#endif

	UCSRB = _BV(RXEN) | _BV(TXEN); /* Enable RX and TX */
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0); /* 8-bit data */

	stdout = &mystdout;
	_delay_ms(500);
	//printf("hello\r\n");

	GICR |= _BV(INT0); // enable INT0 external interrupt
	MCUCR |= _BV(ISC01); // falling edge

	sei();

	while(1)
	{
		_delay_ms(1000);
		printf("positive %4d %4d %4d ",positive[0],positive[1],positive[2]);
		printf("negative %4d %4d %4d ",negative[0],negative[1],negative[2]);
		printf("diff %4d %4d %4d\r\n",negative[0]-positive[0],positive[1]-negative[1],negative[2]-positive[2]);
	}

        while (1)
        {
                loop_until_bit_is_clear(PIND, PD2);
		adc(0x48);
                if (bit_is_clear(PINB,PB2))
                {
                        //printf("positive\r\n");
                        //printf("negative %4d %4d %4d %4d %4d %4d %4d %4d\r\n",adc(0x48), adc(0x49), adc(0x4a), adc(0x4b), adc(0x4c), adc(0x4d), adc(0x4e), adc(0x4f));
                }
                else if(bit_is_set(PINB,PB2))
                {
                        //printf("negative\r\n");
                        //printf("positive %4d %4d %4d %4d %4d %4d %4d %4d\r\n",adc(0x48), adc(0x49), adc(0x4a), adc(0x4b), adc(0x4c), adc(0x4d), adc(0x4e), adc(0x4f));
                }
        }

	return 0;
}

