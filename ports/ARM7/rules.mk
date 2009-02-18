# ARM7 common makefile scripts and rules.

# Automatic compiler options
OPT = $(USE_OPT)
CPPOPT = $(USE_CPPOPT)
ifeq ($(USE_CURRP_CACHING),yes)
  OPT += -ffixed-r7 -DCH_CURRP_REGISTER_CACHE='"r7"'
endif
ifeq ($(USE_LINK_GC),yes)
  OPT += -ffunction-sections -fdata-sections
endif

# Source files groups
ifeq ($(USE_THUMB),yes)
  TCSRC += $(CSRC)
  TCPPSRC += $(CPPSRC)
else
  ACSRC += $(CSRC)
  ACPPSRC += $(CPPSRC)
endif
ASRC	 = $(ACSRC)$(ACPPSRC)
TSRC	 = $(TCSRC)$(TCPPSRC)
SRC	     = $(ASRC)$(TSRC)

# Object files groups
ACOBJS   = $(ACSRC:.c=.o)
ACPPOBJS = $(ACPPSRC:.cpp=.o)
TCOBJS   = $(TCSRC:.c=.o)
TCPPOBJS = $(TCPPSRC:.cpp=.o)
ASMOBJS  = $(ASMSRC:.s=.o)
OBJS	 = $(ASMOBJS) $(ACOBJS) $(TCOBJS) $(ACPPOBJS) $(TCPPOBJS)

# Paths
IINCDIR = $(patsubst %,-I%,$(INCDIR) $(DINCDIR) $(UINCDIR))
LLIBDIR = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

# Macros
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
LIBS    = $(DLIBS) $(ULIBS)

# Various settings
MCFLAGS = -mcpu=$(MCU)
ODFLAGS	= -x --syms
ASFLAGS = $(MCFLAGS) -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CFLAGS   = $(MCFLAGS) $(OPT) $(CWARN) -Wa,-alms=$(<:.c=.lst) $(DEFS)
CPPFLAGS = $(MCFLAGS) $(OPT) $(CPPOPT) $(CPPWARN) -Wa,-alms=$(<:.cpp=.lst) $(DEFS)
ifeq ($(LINK_GC),yes)
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch,--gc-sections $(LLIBDIR)
else
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LLIBDIR)
endif

# Thumb interwork enabled only if needed because it kills performance.
ifneq ($(TSRC),)
  CFLAGS += -DTHUMB_PRESENT
  CPPFLAGS += -DTHUMB_PRESENT
  ASFLAGS += -DTHUMB_PRESENT
  ifneq ($(ASRC),)
    # Mixed ARM and THUMB case.
    CFLAGS += -mthumb-interwork
    CPPFLAGS += -mthumb-interwork
    LDFLAGS += -mthumb-interwork
  else
    # Pure THUMB case, THUMB C code cannot be called by ARM asm code directly.
    CFLAGS += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    CPPFLAGS += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    LDFLAGS += -mno-thumb-interwork -mthumb
    ASFLAGS += -DTHUMB_NO_INTERWORKING
  endif
else
  CPFLAGS += -mno-thumb-interwork
  CPPFLAGS += -mno-thumb-interwork
  LDFLAGS += -mno-thumb-interwork
endif

# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d
CPPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# Makefile rules
#

all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin $(PROJECT).dmp

$(ACPPOBJS) : %.o : %.cpp
	@echo
	$(CPPC) -c $(CPPFLAGS) $(AOPT) -I . $(IINCDIR) $< -o $@

$(TCPPOBJS) : %.o : %.cpp
	@echo
	$(CPPC) -c $(CPPFLAGS) $(TOPT) -I . $(IINCDIR) $< -o $@

$(ACOBJS) : %.o : %.c
	@echo
	$(CC) -c $(CFLAGS) $(AOPT) -I . $(IINCDIR) $< -o $@

$(TCOBJS) : %.o : %.c
	@echo
	$(CC) -c $(CFLAGS) $(TOPT) -I . $(IINCDIR) $< -o $@

$(ASMOBJS) : %.o : %.s
	@echo
	$(AS) -c $(ASFLAGS) -I . $(IINCDIR) $< -o $@

%elf: $(OBJS)
	@echo
	$(LD) $(ASMOBJS) $(ACOBJS) $(TCOBJS) $(ACPPOBJS) $(TCPPOBJS) $(LDFLAGS) $(LIBS) -o $@

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
	-rm -f $(ACSRC:.c=.c.bak)
	-rm -f $(ACSRC:.c=.lst)
	-rm -f $(TCSRC:.c=.c.bak)
	-rm -f $(TCSRC:.c=.lst)
	-rm -f $(ACPPSRC:.cpp=.c.bak)
	-rm -f $(ACPPSRC:.cpp=.lst)
	-rm -f $(TCPPSRC:.cpp=.c.bak)
	-rm -f $(TCPPSRC:.cpp=.lst)
	-rm -f $(ASMSRC:.s=.s.bak)
	-rm -f $(ASMSRC:.s=.lst)
	-rm -fR .dep

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
