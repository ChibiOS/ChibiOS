# List of all the ChibiOS/HAL files, there is no need to remove the files
# from this list, you can disable parts of the HAL by editing halconf.h.
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

HALSRC := $(CHIBIOS)/os/xhal/src/hal.c \
          $(CHIBIOS)/os/xhal/src/hal_base_driver.c \
          $(CHIBIOS)/os/xhal/src/hal_st.c \
          $(CHIBIOS)/os/xhal/src/hal_buffered_serial.c \
          $(CHIBIOS)/os/xhal/src/hal_queues.c
ifneq ($(findstring HAL_USE_PAL TRUE,$(HALCONF)),)
HALSRC += $(CHIBIOS)/os/xhal/src/hal_pal.c
endif
ifneq ($(findstring HAL_USE_SIO TRUE,$(HALCONF)),)
HALSRC += $(CHIBIOS)/os/xhal/src/hal_sio.c
endif
else
HALSRC = $(CHIBIOS)/os/xhal/src/hal.c \
         $(CHIBIOS)/os/xhal/src/hal_base_driver.c \
         $(CHIBIOS)/os/xhal/src/hal_st.c \
         $(CHIBIOS)/os/xhal/src/hal_queues.c \
         $(CHIBIOS)/os/xhal/src/hal_pal.c \
         $(CHIBIOS)/os/xhal/src/hal_sio.c
endif

# Required include directories
HALINC = $(CHIBIOS)/os/xhal/include

# Shared variables
ALLCSRC += $(HALSRC)
ALLINC  += $(HALINC)
