# Required platform files.
PLATFORMSRC := $(CHIBIOS)/os/hal/ports/common/ARMCMx/nvic.c \
               $(CHIBIOS)/os/hal/ports/STM32/STM32F7xx/stm32_isr.c \
               $(CHIBIOS)/os/hal/ports/STM32/STM32F7xx/hal_lld.c \
               $(CHIBIOS)/os/hal/ports/STM32/STM32F7xx/hal_efl_lld.c

# Required include directories.
PLATFORMINC := $(CHIBIOS)/os/hal/ports/common/ARMCMx \
               $(CHIBIOS)/os/hal/ports/STM32/STM32F7xx

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
include $(CHIBIOS)/os/hal/ports/STM32/LLD/ADCv2/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/CANv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/CRYPv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/DACv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/DMAv2/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/EXTIv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/GPIOv2/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/I2Cv2/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/MACv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/OTGv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/QUADSPIv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/RNGv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/RTCv2/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/SPIv2/driver_v2.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/SDMMCv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/SYSTICKv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/TIMv1/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/USARTv2/driver.mk
include $(CHIBIOS)/os/hal/ports/STM32/LLD/xWDGv1/driver.mk

# Shared variables
ALLCSRC += $(PLATFORMSRC)
ALLINC  += $(PLATFORMINC)
