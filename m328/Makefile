all:	serial.hex blink.hex

nall:	blink.c
	avr-gcc -g -Os -mmcu=atmega328 -c blink.c
	avr-gcc -g -mmcu=atmega328 -o blink.elf blink.o
	avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex
	avrdude -c avrisp2 -p m328 -U blink.hex

CC	= avr-gcc
CFLAGS	= -Os -mmcu=atmega328

%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $^

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

%.isp:	%.hex
	avrdude -c avrisp2 -p m328 -U $^


