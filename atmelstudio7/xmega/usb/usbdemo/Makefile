##############################################################################
# Generic Makefile
###############################################################################

OUTPUT = default

# General Flags
TARGET = $(PROJECT).exe
CC = gcc.exe
MAKECFG   = config.mk
RC = windres.exe

# Options common to compile, link and assembly rules
COMMON =

# Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS +=


# Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS +=

# Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -lgdi32

# Include Directories
INCLUDES = -I"./"

# Include Source files list and part informations
include $(MAKECFG)


## Build
.PHONY: all
all: $(TARGET)

## Clean target
.PHONY: clean
clean:
	@echo "Clean project"
	@-rm -rf $(OUTPUT)/dep/* $(OUTPUT)/* $(PROJECT).exe

## Rebuild the project.
.PHONY: rebuild
rebuild: clean all


## Compile

# Create objects files list with sources files
OBJECTS  = $(CSRCS:.c=.o) $(ASSRCS:.s=.o) $(RCSRCS:.rc=.o)

.PHONY: objfiles
objfiles: $(OBJECTS)

# create object files from C source files.
%.o: %.c
	@echo 'Building file: $<'
	@$(shell mkdir $(OUTPUT) 2>/dev/null)
	@$(shell mkdir $(OUTPUT)/dep 2>/dev/null)
	@$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $(OUTPUT)/$(@F)

# Preprocess & assemble: create object files from assembler source files.
%.o: %.s
	@echo 'Building file: $<'
	@$(shell mkdir $(OUTPUT) 2>/dev/null)
	@$(shell mkdir $(OUTPUT)/dep 2>/dev/null)
	@$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $(OUTPUT)/$(@F)

# create object files from resource files.
%.o: %.rc
	@echo 'Building file: $<'
	@$(shell mkdir $(OUTPUT) 2>/dev/null)
	@$(shell mkdir $(OUTPUT)/dep 2>/dev/null)
	@$(RC) -i $< -o $(OUTPUT)/$(@F)

## Link
$(TARGET): $(OBJECTS)
	@echo "Linking"
	@$(CC) $(LDFLAGS) $(addprefix $(OUTPUT)/,$(notdir $(OBJECTS))) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET) -mwindows -Wl,--subsystem,windows

%.lss: $(TARGET)
	@echo "Create lss file"
	@avr-objdump -h -S $< > $@
