all:	serial.hex blink.hex

nall:	blink.c
	avr-gcc -g -Os -mmcu=atmega8515 -c blink.c
	avr-gcc -g -mmcu=atmega8515 -o blink.elf blink.o
	avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex
	avrdude -c avrisp2 -p m8515 -U blink.hex

CC	= avr-gcc
CFLAGS	= -Os -mmcu=atmega8515

%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $^

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

%.isp:	%.hex
	avrdude -c avrisp2 -p m8515 -U $^


