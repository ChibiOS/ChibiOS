# XHAL subsystem build.

# Dependencies.
include $(CHIBIOS)/os/common/oop/oop.mk

ifeq ($(USE_SMART_BUILD),yes)

# Configuration files directory
ifeq ($(XHALCONFDIR),)
  ifeq ($(CONFDIR),)
    XHALCONFDIR = .
  else
    XHALCONFDIR := $(CONFDIR)
  endif
endif

XHALCONF := $(strip $(shell cat $(XHALCONFDIR)/xhalconf.h | grep -E "\#define"))

# Required files.
XHALSRC := $(CHIBIOS)/os/xhal/src/hal.c \
           $(CHIBIOS)/os/xhal/src/hal_safety.c \
           $(CHIBIOS)/os/xhal/src/hal_base_driver.c \
           $(CHIBIOS)/os/xhal/src/hal_cb_driver.c \
           $(CHIBIOS)/os/xhal/src/hal_st.c \
           $(CHIBIOS)/os/xhal/src/hal_buffered_serial.c \
           $(CHIBIOS)/os/xhal/src/hal_queues.c
ifneq ($(findstring HAL_USE_ETH TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_eth.c
endif
ifneq ($(findstring HAL_USE_PAL TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_pal.c
endif
ifneq ($(findstring HAL_USE_SIO TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_sio.c
endif
ifneq ($(findstring HAL_USE_SPI TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_spi.c
endif
else
XHALSRC = $(CHIBIOS)/os/xhal/src/hal.c \
          $(CHIBIOS)/os/xhal/src/hal_base_driver.c \
          $(CHIBIOS)/os/xhal/src/hal_st.c \
          $(CHIBIOS)/os/xhal/src/hal_queues.c \
          $(CHIBIOS)/os/xhal/src/hal_eth.c \
          $(CHIBIOS)/os/xhal/src/hal_pal.c \
          $(CHIBIOS)/os/xhal/src/hal_sio.c \
          $(CHIBIOS)/os/xhal/src/hal_spi.c
endif

# Required include directories
XHALINC = $(CHIBIOS)/os/xhal/include

# Shared variables
ALLCSRC += $(XHALSRC)
ALLINC  += $(XHALINC)
