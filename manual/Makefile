all:	blink.c
	avr-gcc -g -Os -mmcu=atmega162 -c blink.c
	avr-gcc -g -mmcu=atmega162 -o blink.elf blink.o
	avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex
	avrdude -c avrisp2 -p m162 -U blink.hex

CC	= avr-gcc
CFLAGS	= -Os -mmcu=atmega162

%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $^

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

blink:	blink.hex
	avrdude -c avrisp2 -p m162 -U blink.hex


