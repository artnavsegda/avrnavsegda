hello.elf:	hello.c
	avr-gcc -Os -mmcu=atmega328p hello.c -o hello.elf

upoload:	hello.elf
	avrdude -c arduino -p m328p -P /dev/ttyUSB0 -U hello.elf

clean:
	rm hello.elf

