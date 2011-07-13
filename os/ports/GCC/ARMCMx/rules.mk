# ARM Cortex-Mx common makefile scripts and rules.

# Output
OUTFILES = $(BUILDDIR)/$(PROJECT).elf $(BUILDDIR)/$(PROJECT).hex $(BUILDDIR)/$(PROJECT).bin $(BUILDDIR)/$(PROJECT).dmp
ifeq ($(BUILDDIR),)
  BUILDDIR = .
  CLEANDIR =
else
  CLEANDIR = $(BUILDDIR)
endif
ENSUREBUILDDIR = $(shell test -d $(BUILDDIR) || mkdir $(BUILDDIR))

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

# Libs
LIBS    = $(DLIBS) $(ULIBS)

# Various settings
MCFLAGS = -mcpu=$(MCU)
ODFLAGS	= -x --syms
ASFLAGS = $(MCFLAGS) -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CFLAGS   = $(MCFLAGS) $(OPT) $(CWARN) -Wa,-alms=$(<:.c=.lst) $(DEFS)
CPPFLAGS = $(MCFLAGS) $(OPT) $(CPPOPT) $(CPPWARN) -Wa,-alms=$(<:.cpp=.lst) $(DEFS)
ifeq ($(USE_LINK_GC),yes)
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(BUILDDIR)/$(PROJECT).map,--cref,--no-warn-mismatch,--gc-sections $(LLIBDIR)
else
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(BUILDDIR)/$(PROJECT).map,--cref,--no-warn-mismatch $(LLIBDIR)
endif

# Thumb interwork enabled only if needed because it kills performance.
ifneq ($(TSRC),)
  CFLAGS += -DTHUMB_PRESENT
  CPPFLAGS += -DTHUMB_PRESENT
  ASFLAGS += -DTHUMB_PRESENT
  ifneq ($(ASRC),)
    # Mixed ARM and THUMB mode.
    CFLAGS += -mthumb-interwork
    CPPFLAGS += -mthumb-interwork
    ASFLAGS += -mthumb-interwork
    LDFLAGS += -mthumb-interwork
  else
    # Pure THUMB mode, THUMB C code cannot be called by ARM asm code directly.
    CFLAGS += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    CPPFLAGS += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    ASFLAGS += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING -mthumb
    LDFLAGS += -mno-thumb-interwork -mthumb
  endif
else
  # Pure ARM mode
  CFLAGS += -mno-thumb-interwork
  CPPFLAGS += -mno-thumb-interwork
  ASFLAGS += -mno-thumb-interwork
  LDFLAGS += -mno-thumb-interwork
endif

# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d
CPPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# Makefile rules
#

all: $(ENSUREBUILDDIR) $(OBJS) $(OUTFILES) MAKE_ALL_RULE_HOOK

MAKE_ALL_RULE_HOOK:

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
	$(LD) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN) $< $@

%dmp: %elf
	$(OD) $(ODFLAGS) $< > $@

clean:
	-rm -f $(OBJS)
	-rm -f $(ACSRC:.c=.lst) $(TCSRC:.c=.lst) $(ACPPSRC:.cpp=.lst) $(TCPPSRC:.cpp=.lst) $(ASMSRC:.s=.lst)
	-rm -f $(OUTFILES) $(BUILDDIR)/$(PROJECT).map
	-rm -fR .dep $(CLEANDIR)

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
