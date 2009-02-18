# ARM7 common makefile rules.

OPT = $(USE_OPT)

ifeq ($(USE_CURRP_CACHING),yes)
  OPT += -ffixed-r7 -DCH_CURRP_REGISTER_CACHE='"r7"'
endif

ifeq ($(USE_LINK_GC),yes)
  OPT += -ffunction-sections -fdata-sections
endif

ifeq ($(USE_THUMB),yes)
  TSRC += $(SRC)
else
  ASRC += $(SRC)
endif

IINCDIR = $(patsubst %,-I%,$(INCDIR) $(DINCDIR) $(UINCDIR))
LLIBDIR = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
AOBJS   = $(ASRC:.c=.o)
TOBJS   = $(TSRC:.c=.o)
OBJS	= $(ASMOBJS) $(AOBJS) $(TOBJS)
ASMOBJS = $(ASMSRC:.s=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(MCU)

ODFLAGS	= -x --syms
ASFLAGS = $(MCFLAGS) -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) $(WARN) -Wa,-alms=$(<:.c=.lst) $(DEFS)
ifeq ($(LINK_GC),yes)
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch,--gc-sections $(LLIBDIR)
else
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LLIBDIR)
endif

# Thumb interwork enabled only if needed because it kills performance.
ifneq ($(TSRC),)
  CPFLAGS += -DTHUMB_PRESENT
  ASFLAGS += -DTHUMB_PRESENT
  ifneq ($(ASRC),)
    # Mixed ARM and THUMB case.
    CPFLAGS += -mthumb-interwork
    LDFLAGS += -mthumb-interwork
  else
    # Pure THUMB case, THUMB C code cannot be called by ARM asm code directly.
    CPFLAGS += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    LDFLAGS += -mno-thumb-interwork -mthumb
    ASFLAGS += -DTHUMB_NO_INTERWORKING
  endif
else
  CPFLAGS += -mno-thumb-interwork
  LDFLAGS += -mno-thumb-interwork
endif

# Generate dependency information
CPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# Makefile rules
#

all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin $(PROJECT).dmp

$(AOBJS) : %.o : %.c
	@echo
	$(CC) -c $(CPFLAGS) $(AOPT) -I . $(IINCDIR) $< -o $@

$(TOBJS) : %.o : %.c
	@echo
	$(CC) -c $(CPFLAGS) $(TOPT) -I . $(IINCDIR) $< -o $@

$(ASMOBJS) : %.o : %.s
	@echo
	$(AS) -c $(ASFLAGS) -I . $(IINCDIR) $< -o $@

%elf: $(OBJS)
	@echo
	$(CC) $(ASMOBJS) $(AOBJS) $(TOBJS) $(LDFLAGS) $(LIBS) -o $@

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
	-rm -f $(ASRC:.c=.c.bak)
	-rm -f $(ASRC:.c=.lst)
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
