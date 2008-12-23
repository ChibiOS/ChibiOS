#
#       !!!! Do NOT edit this makefile with an editor which replace tabs by spaces !!!!
#
##############################################################################################
#
# On command line:
#
# make all = Create project
#
# make clean = Clean project files.
#
# To rebuild project do "make clean" and "make all".
#

##############################################################################################
# Start of default section
#

TRGT = msp430-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

MCU  = msp430x1611

# List all default C defines here, like -D_DEBUG=1
DDEFS =

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =

#
# End of default section
##############################################################################################

##############################################################################################
# Start of user section
#

# Define project name here
PROJECT = ch

# Define linker script file here
LDSCRIPT= mspgcc/msp430x1611.x

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# Imported source files
include ../../src/kernel.mk
include ../../test/test.mk

# List ARM-mode C source files here
SRC = ../../ports/MSP430/chcore.c \
      ../../ports/MSP430/msp430_serial.c \
       ${KERNSRC} \
       ${TESTSRC} \
      ../../src/lib/evtimer.c \
      board.c main.c

# List ASM source files here
ASMSRC =

# List all user directories here
UINCDIR = ../../src/include ../../src/lib ../../test ../../ports/MSP430

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

# Common options here
# NOTE: -ffixed-r7 is only needed if you enabled CH_CURRP_REGISTER_CACHE in
#       chconf.h.
OPT = -O2 -ggdb -fomit-frame-pointer
#OPT += -ffixed-r7

# Define warning options here
WARN = -Wall -Wstrict-prototypes

#
# End of user defines
##############################################################################################

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
COBJS   = $(SRC:.c=.o)
ASMOBJS = $(ASMSRC:.s=.o)
OBJS	= $(ASMOBJS) $(COBJS)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mmcu=$(MCU)

ASFLAGS = $(MCFLAGS) -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) $(WARN) -Wa,-alms=$(<:.c=.lst) $(DEFS)
LDFLAGS = $(MCFLAGS) -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR)
ODFLAGS	= -x --syms

# Generate dependency information
CPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# Makefile rules
#
all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin $(PROJECT).dmp

$(COBJS) : %.o : %.c
	@echo
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@

$(ASMOBJS) : %.o : %.s
	@echo
	$(AS) -c $(ASFLAGS) -I . $(INCDIR) $< -o $@

%elf: $(OBJS)
	@echo
	$(CC) $(ASMOBJS) $(COBJS) $(LDFLAGS) $(LIBS) -o $@

%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN) $< $@

%dmp: %elf
	$(OD) $(ODFLAGS) $< > $@

clean:
	-rm -f $(OBJS)
	-rm -f $(PROJECT).elf
	-rm -f $(PROJECT).dmp
	-rm -f $(PROJECT).map
	-rm -f $(PROJECT).hex
	-rm -f $(PROJECT).bin
	-rm -f $(SRC:.c=.c.bak)
	-rm -f $(SRC:.c=.lst)
	-rm -f $(ASMSRC:.s=.s.bak)
	-rm -f $(ASMSRC:.s=.lst)
	-rm -fR .dep

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
