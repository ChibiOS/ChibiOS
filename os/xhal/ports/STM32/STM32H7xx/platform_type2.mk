# Required platform files.
PLATFORMSRC := $(CHIBIOS)/os/xhal/ports/common/ARMCMx/nvic.c \
               $(CHIBIOS)/os/xhal/ports/STM32/STM32H7xx/stm32_isr.c \
               $(CHIBIOS)/os/xhal/ports/STM32/STM32H7xx/hal_lld.c

# Required include directories.
PLATFORMINC := $(CHIBIOS)/os/xhal/ports/common/ARMCMx \
               $(CHIBIOS)/os/xhal/ports/STM32/STM32H7xx

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
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/BDMAv1/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/DMAv2/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/EXTIv1/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/GPIOv2/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/MDMAv1/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/OCTOSPIv2/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/RCCv1/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/SYSTICKv1/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/TIMv1/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/USARTv3/driver.mk
include $(CHIBIOS)/os/xhal/ports/STM32/LLD/xWDGv1/driver.mk

# Shared variables
ALLCSRC += $(PLATFORMSRC)
ALLINC  += $(PLATFORMINC)
