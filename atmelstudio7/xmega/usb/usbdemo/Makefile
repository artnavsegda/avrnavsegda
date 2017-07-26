##############################################################################
# Generic Makefile
###############################################################################

OUTPUT = default

# General Flags
TARGET = $(PROJECT)
CC = gcc
MAKECFG   = config.mk

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
LDFLAGS += -lusb

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
OBJECTS  = $(CSRCS:.c=.o) $(ASSRCS:.s=.o)

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


## Link
$(TARGET): $(OBJECTS)
	@echo "Linking"
	@$(CC) $(LDFLAGS) $(addprefix $(OUTPUT)/,$(notdir $(OBJECTS))) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.lss: $(TARGET)
	@echo "Create lss file"
	@avr-objdump -h -S $< > $@


