###############################################################################
# Makefile for the project usb
###############################################################################

## General Flags
PROJECT = usb
MCU = atxmega256a3bu
TARGET = usb.elf
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
LDFLAGS +=  -Wl,-Map=usb.map
LDFLAGS += -Wl,-section-start=.BOOT=0x40000


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\." -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\adc" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\utils" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\utils" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\utils\preprocessor" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\config" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\nvm" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\cpu" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\sleepmgr" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\sleep" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\clock" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\boards" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\boards" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\delay" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\gpio" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\ioport" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\pmic" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\spi" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\spi" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\utils\stdio\stdio_serial" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\serial" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\usart" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\tc" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\twi" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\drivers\twi" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\xmega\boards\xmega_a3bu_xplained" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\usb" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\usb\udc" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\usb\class\vendor\device" -I"Z:\GitHub\avrnavsegda\avrstudiov4\xmega\usb\usb\ASF\common\services\usb\class\vendor" 

## Objects that must be built in order to link
OBJECTS = main.o usart.o twim.o adc.o adc_aau.o write.o read.o spi_master.o sleepmgr.o usart_serial.o sysclk.o tc.o spi.o nvm_asm.o nvm.o ccp.o ioport_compat.o init.o udi_vendor.o udi_vendor_desc.o udc.o usb_device.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) usb.hex usb.eep usb.lss size

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

udi_vendor.o: ../ASF/common/services/usb/class/vendor/device/udi_vendor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

udi_vendor_desc.o: ../ASF/common/services/usb/class/vendor/device/udi_vendor_desc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

udc.o: ../ASF/common/services/usb/udc/udc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usb_device.o: ../ASF/xmega/drivers/usb/usb_device.c
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
	-rm -rf $(OBJECTS) usb.elf dep/* usb.hex usb.eep usb.lss usb.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

