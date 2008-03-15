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

TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

MCU  = cortex-m3

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
LDSCRIPT= ch.ld

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List ARM-mode C source files here
SRC = ../../ports/ARMCM3/chcore.c \
      ../../src/chinit.c ../../src/chdebug.c ../../src/chlists.c ../../src/chdelta.c \
      ../../src/chschd.c ../../src/chthreads.c ../../src/chsem.c ../../src/chmtx.c \
      ../../src/chevents.c ../../src/chmsg.c ../../src/chsleep.c ../../src/chqueues.c \
      ../../src/chserial.c \
      ../../src/lib/evtimer.c ../../test/test.c \
      board.c main.c

# List ASM source files here
ASMSRC = ../../ports/ARMCM3/crt0.s

# List all user directories here
UINCDIR = ../../src/include ../../src/lib ../../ports/ARMCM3

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

# Common options here
# NOTE: -ffixed-r7 is only needed if you enabled CH_CURRP_REGISTER_CACHE in
#       chconf.h.
# NOTE: -falign-functions=16 may improve the performance, not always, but
#       increases the code size.
OPT = -O2 -ggdb -fomit-frame-pointer
#OPT += -ffixed-r7
#OPT += -falign-functions=16

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
MCFLAGS = -mcpu=$(MCU) -mthumb

ASFLAGS = $(MCFLAGS) -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) $(WARN) -Wa,-alms=$(<:.c=.lst) $(DEFS)
LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR)
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
