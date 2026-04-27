# Required platform files.
PLATFORMSRC := $(CHIBIOS)/os/xhal/ports/vio/hal_lld.c \
               $(CHIBIOS)/os/xhal/ports/vio/hal_st_lld.c \
               $(CHIBIOS)/os/xhal/ports/vio/hal_pal_lld.c \
               $(CHIBIOS)/os/xhal/ports/vio/hal_eth_lld.c \
               $(CHIBIOS)/os/xhal/ports/vio/hal_sio_lld.c \
               $(CHIBIOS)/os/xhal/ports/vio/hal_spi_lld.c

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

HALCONF := $(strip $(shell cat $(HALCONFDIR)/xhalconf.h | grep -E "\#define"))

else
endif

# Drivers compatible with the platform.
ifneq ($(findstring HAL_USE_RTC TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/vio/hal_rtc_lld.c
endif
ifneq ($(findstring HAL_USE_ADC TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/vio/hal_adc_lld.c
endif
ifneq ($(findstring HAL_USE_GPT TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/vio/hal_gpt_lld.c
endif
ifneq ($(findstring HAL_USE_I2C TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/vio/hal_i2c_lld.c
endif

# Shared variables
ALLCSRC += $(PLATFORMSRC)
ALLINC  += $(PLATFORMINC)
