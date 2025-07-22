# Required platform files.
PLATFORMSRC := $(CHIBIOS)/os/hal/ports/common/ARMCMx/nvic.c \
               $(CHIBIOS)/os/hal/ports/RP/RP2350/rp_isr.c \
               $(CHIBIOS)/os/hal/ports/RP/RP2350/hal_lld.c

# Required include directories.
PLATFORMINC := $(CHIBIOS)/os/hal/ports/common/ARMCMx \
               $(CHIBIOS)/os/hal/ports/RP/RP2350

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
# buzz is using a mix of drivers, incl some of karls v2 driver/s work., but they might need more testing? 
# the only v1 driver we use: 
include $(CHIBIOS)/os/hal/ports/RP/LLD/UARTv1/driver.mk

# karls v2 drivers.
include $(CHIBIOS)/os/hal/ports/RP/LLD/DMAv2/driver.mk
include $(CHIBIOS)/os/hal/ports/RP/LLD/GPIOv2/driver.mk
include $(CHIBIOS)/os/hal/ports/RP/LLD/TIMERv2/driver.mk
# todo we dont have a SPI, RTC, or WDG driver yet, will they be v1 or v2?   v1 means works-with-2040 (or both), v2 means works-only-with-2350 - ish.
# include $(CHIBIOS)/os/hal/ports/RP/LLD/SPIvX/driver.mk
# include $(CHIBIOS)/os/hal/ports/RP/LLD/RTCvX/driver.mk
# include $(CHIBIOS)/os/hal/ports/RP/LLD/WDGvX/driver.mk

# Shared variables
ALLCSRC += $(PLATFORMSRC)
ALLINC  += $(PLATFORMINC)
