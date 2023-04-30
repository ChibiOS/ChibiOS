# Required platform files.
PLATFORMSRC := $(CHIBIOS)/os/xhal/ports/vio/hal_lld.c \
               $(CHIBIOS)/os/xhal/ports/vio/hal_st_lld.c \
               $(CHIBIOS)/os/xhal/ports/vio/hal_pal_lld.c \
               $(CHIBIOS)/os/xhal/ports/vio/hal_sio_lld.c

# Required include directories.
PLATFORMINC := $(CHIBIOS)/os/xhal/ports/vio

# Optional platform files.
ifeq ($(USE_SMART_BUILD),yes)

# Configuration files directory
ifeq ($(HALCONFDIR),)
  ifeq ($(CONFDIR),)
    HALCONFDIR = .
  else
    HALCONFDIR := $(CONFDIR)
  endif
endif

HALCONF := $(strip $(shell cat $(HALCONFDIR)/halconf.h | egrep -e "\#define"))

else
endif

# Drivers compatible with the platform.

# Shared variables
ALLCSRC += $(PLATFORMSRC)
ALLINC  += $(PLATFORMINC)
