# List of all the ChibiOS/LIB files, there is no need to remove the files
# from this list, you can disable parts of the kernel by editing chlibconf.h.
ifeq ($(USE_SMART_BUILD),yes)

# Configuration files directory
ifeq ($(CONFDIR),)
  CONFDIR = .
endif

CHLIBCONF := $(strip $(shell cat $(CONFDIR)/chconf.h | egrep -e "\#define"))

LIBSRC :=
ifneq ($(findstring CH_CFG_USE_MAILBOXES TRUE,$(CHLIBCONF)),)
LIBSRC += $(CHIBIOS)/os/lib/src/chmboxes.c
endif
ifneq ($(findstring CH_CFG_USE_MEMCORE TRUE,$(CHLIBCONF)),)
LIBSRC += $(CHIBIOS)/os/lib/src/chmemcore.c
endif
ifneq ($(findstring CH_CFG_USE_HEAP TRUE,$(CHLIBCONF)),)
LIBSRC += $(CHIBIOS)/os/lib/src/chmemheaps.c
endif
ifneq ($(findstring CH_CFG_USE_MEMPOOLS TRUE,$(CHLIBCONF)),)
LIBSRC += $(CHIBIOS)/os/lib/src/chmempools.c
endif
ifneq ($(findstring CH_CFG_USE_FACTORY TRUE,$(CHLIBCONF)),)
LIBSRC += $(CHIBIOS)/os/lib/src/chfactory.c
endif
else
LIBSRC := $(CHIBIOS)/os/lib/src/chmboxes.c \
          $(CHIBIOS)/os/lib/src/chmemcore.c \
          $(CHIBIOS)/os/lib/src/chmemheaps.c \
          $(CHIBIOS)/os/lib/src/chmempools.c \
          $(CHIBIOS)/os/lib/src/chfactory.c
endif

# Required include directories
LIBINC := $(CHIBIOS)/os/lib/include

# Shared variables
ALLCSRC += $(LIBSRC)
ALLINC  += $(LIBINC)
