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

TRGT = arm-elf-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

MCU  = arm7tdmi

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
CSRC = ../../ports/ARM7-LPC214x/chcore.c \
       ../../ports/ARM7-LPC214x/vic.c  \
       ../../ports/ARM7-LPC214x/lpc214x_serial.c  \
       ../../src/chinit.c ../../src/chdebug.c ../../src/chlists.c ../../src/chdelta.c \
       ../../src/chschd.c ../../src/chthreads.c ../../src/chsem.c ../../src/chmtx.c \
       ../../src/chevents.c ../../src/chmsg.c ../../src/chsleep.c ../../src/chqueues.c \
       ../../src/chserial.c \
       ../../src/lib/evtimer.c ../../test/test.c \
       board.c

# List ARM-mode C++ source files here
CPPSRC = ../../src/lib/ch.cpp main.cpp

# List THUMB-mode C sources here
# NOTE: If any module is compiled in thumb mode then -mthumb-interwork is
#       enabled for all modules and that lowers performance.
TSRC =

# List ASM source files here
ASMSRC = ../../ports/ARM7-LPC214x/crt0.s ../../ports/ARM7/chsys.s

# List all user directories here
UINCDIR = ../../src/include ../../src/lib ../../test \
          ../../ports/ARM7 ../../ports/ARM7-LPC214x

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -D THUMB

# Common options here
# NOTE: -ffixed-r7 is only needed if you enabled CH_CURRP_REGISTER_CACHE in
#       chconf.h.
# NOTE: -falign-functions=16 may improve the performance, not always, but
#       increases the code size.
OPT = -O2 -ggdb -fomit-frame-pointer
#OPT += -ffixed-r7
OPT += -falign-functions=16

# C++ specific options here
# NOTE: -fno-rtti saves a LOT of code space, remove it only if you really need
#       RTTI.
CPPOPT = -fno-rtti

# Define warning options here
WARN = -Wall

#
# End of user defines
##############################################################################################

INCDIR   = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR   = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS     = $(DDEFS) $(UDEFS)
ADEFS    = $(DADEFS) $(UADEFS)
AOBJS    = $(CSRC:.c=.o)
TOBJS    = $(TSRC:.c=.o)
CPPOBJS  = $(CPPSRC:.cpp=.o)
ASMOBJS  = $(ASMSRC:.s=.o)
OBJS	 = $(ASMOBJS) $(CPPOBJS) $(AOBJS) $(TOBJS)
LIBS     = $(DLIBS) $(ULIBS)
MCFLAGS  = -mcpu=$(MCU)

ASFLAGS  = $(MCFLAGS) -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CFLAGS   = $(MCFLAGS) $(OPT) $(WARN) -Wa,-alms=$(<:.c=.lst) $(DEFS)
CPPFLAGS = $(MCFLAGS) $(OPT) $(CPPOPT) $(WARN) -Wa,-alms=$(<:.cpp=.lst) $(DEFS)
LDFLAGS  = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR)
ODFLAGS  = -x --syms

# Thumb interwork enabled only if needed because it kills performance.
ifneq ($(TSRC),)
  CFLAGS += -D THUMB_PRESENT
  CPPFLAGS += -D THUMB_PRESENT
  ASFLAGS += -D THUMB_PRESENT
  ifneq ($(CSRC),)
    # Mixed ARM and THUMB case.
    CFLAGS += -mthumb-interwork
    CPPFLAGS += -mthumb-interwork
    LDFLAGS += -mthumb-interwork
  else
    # Pure THUMB case, THUMB C code cannot be called by ARM asm code directly.
    CFLAGS += -D THUMB_NO_INTERWORKING
    CPPFLAGS += -D THUMB_NO_INTERWORKING
    LDFLAGS += -mthumb
    ASFLAGS += -D THUMB_NO_INTERWORKING
  endif
endif

# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d
CPPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# Makefile rules
#

all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin $(PROJECT).dmp

$(CPPOBJS) : %.o : %.cpp
	@echo
	$(CPPC) -c $(CPPFLAGS) $(AOPT) -I . $(INCDIR) $< -o $@

$(AOBJS) : %.o : %.c
	@echo
	$(CC) -c $(CFLAGS) $(AOPT) -I . $(INCDIR) $< -o $@

$(TOBJS) : %.o : %.c
	@echo
	$(CC) -c $(CFLAGS) $(TOPT) -I . $(INCDIR) $< -o $@

$(ASMOBJS) : %.o : %.s
	@echo
	$(AS) -c $(ASFLAGS) -I . $(INCDIR) $< -o $@

%elf: $(OBJS)
	@echo
	$(LD) $(ASMOBJS) $(AOBJS) $(TOBJS) $(CPPOBJS) $(LDFLAGS) $(LIBS) -o $@

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
	-rm -f $(CSRC:.c=.c.bak)
	-rm -f $(CSRC:.c=.lst)
	-rm -f $(CPPSRC:.cpp=.c.bak)
	-rm -f $(CPPSRC:.cpp=.lst)
	-rm -f $(TSRC:.c=.c.bak)
	-rm -f $(TSRC:.c=.lst)
	-rm -f $(ASMSRC:.s=.s.bak)
	-rm -f $(ASMSRC:.s=.lst)
	-rm -fR .dep

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
