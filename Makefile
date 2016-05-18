all:	xhello.elf hello.elf

xhello.elf:	xhello.c
	avr-gcc -Os -mmcu=atxmega256a3bu xhello.c -o xhello.elf

hello.elf:	hello.c
	avr-gcc -Os -mmcu=atmega328p hello.c -o hello.elf

xupload:	xhello.elf
	avrdude -c avrisp2 -p x256a3b -v -U xhello.elf

upload:	hello.elf
	avrdude -c arduino -p m328p -P /dev/ttyUSB0 -U hello.elf

clean:
	rm hello.elf xhello.elf

