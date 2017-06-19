###############################################################################
# Makefile for the project asf_sequencer
###############################################################################

## General Flags
PROJECT = asf_sequencer
MCU = atxmega256a3bu
TARGET = asf_sequencer.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -DBOARD=XMEGA_A3BU_XPLAINED -DIOPORT_XMEGA_COMPAT 
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=asf_sequencer.map
LDFLAGS += -Wl,-section-start=.BOOT=0x40000


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\." -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\adc" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\utils" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\utils" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\utils\preprocessor" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\config" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\nvm" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\cpu" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\services\sleepmgr" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\sleep" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\services\clock" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\boards" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\boards" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\services\delay" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\services\gpio" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\services\ioport" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\pmic" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\services\spi" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\spi" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\utils\stdio\stdio_serial" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\services\serial" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\usart" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\tc" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\common\services\twi" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\drivers\twi" -I"C:\Users\artna\Documents\GitHub\avrnavsegda\avrstudiov4\xmega\asf_sequencer\asf_sequencer\ASF\xmega\boards\xmega_a3bu_xplained" 

## Objects that must be built in order to link
OBJECTS = main.o usart.o twim.o adc.o adc_aau.o write.o read.o spi_master.o sleepmgr.o usart_serial.o sysclk.o tc.o spi.o nvm_asm.o nvm.o ccp.o ioport_compat.o init.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) asf_sequencer.hex asf_sequencer.eep asf_sequencer.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usart.o: ../ASF/xmega/drivers/usart/usart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

twim.o: ../ASF/xmega/drivers/twi/twim.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc.o: ../ASF/xmega/drivers/adc/adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc_aau.o: ../ASF/xmega/drivers/adc/xmega_aau/adc_aau.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

write.o: ../ASF/common/utils/stdio/write.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

read.o: ../ASF/common/utils/stdio/read.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi_master.o: ../ASF/common/services/spi/xmega_spi/spi_master.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sleepmgr.o: ../ASF/common/services/sleepmgr/xmega/sleepmgr.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usart_serial.o: ../ASF/common/services/serial/usart_serial.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sysclk.o: ../ASF/common/services/clock/xmega/sysclk.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tc.o: ../ASF/xmega/drivers/tc/tc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi.o: ../ASF/xmega/drivers/spi/spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

nvm_asm.o: ../ASF/xmega/drivers/nvm/nvm_asm.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

nvm.o: ../ASF/xmega/drivers/nvm/nvm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ccp.o: ../ASF/xmega/drivers/cpu/ccp.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

ioport_compat.o: ../ASF/common/services/ioport/xmega/ioport_compat.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

init.o: ../ASF/xmega/boards/xmega_a3bu_xplained/init.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) asf_sequencer.elf dep/* asf_sequencer.hex asf_sequencer.eep asf_sequencer.lss asf_sequencer.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

