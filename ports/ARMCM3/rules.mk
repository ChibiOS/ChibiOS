# ARM7 common makefile rules.

OPT = $(USE_OPT)

ifeq ($(USE_CURRP_CACHING),yes)
OPT += -ffixed-r7 -DCH_CURRP_REGISTER_CACHE='"r7"'
endif

ifeq ($(USE_LINK_GC),yes)
  OPT += -ffunction-sections -fdata-sections
endif

ifeq ($(USE_FWLIB),yes)
  include ./stm32lib/stm32lib.mk
  SRC += ${STM32SRC}
  # The thing generates a lot of aliasing warnings, this disables an
  # optimization and the warning disappears, the code is a bit larger however.
  OPT += -fno-strict-aliasing
endif

XINCDIR = $(patsubst %,-I%,$(INCDIR) $(DINCDIR) $(UINCDIR))
XLIBDIR = $(patsubst %,-L%,$(LIBDIR) $(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
COBJS   = $(SRC:.c=.o)
ASMOBJS = $(ASMSRC:.s=.o)
OBJS	= $(ASMOBJS) $(COBJS)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(MCU) -mthumb

ODFLAGS	= -x --syms
ASFLAGS = $(MCFLAGS) -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) $(WARN) -Wa,-alms=$(<:.c=.lst) $(DEFS)
ifeq ($(LINK_GC),yes)
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch,--gc-sections $(XLIBDIR)
else
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(XLIBDIR)
endif

# Generate dependency information
CPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# Makefile rules
#
all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin $(PROJECT).dmp

$(COBJS) : %.o : %.c
	@echo
	$(CC) -c $(CPFLAGS) -I . $(XINCDIR) $< -o $@

$(ASMOBJS) : %.o : %.s
	@echo
	$(AS) -c $(ASFLAGS) -I . $(XINCDIR) $< -o $@

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
