# Required platform files.

PLATFORMSRC := $(CHIBIOS)/os/hal/ports/SAMA/SAMA5D2x/hal_lld.c \
               $(CHIBIOS)/os/hal/ports/SAMA/SAMA5D2x/hal_st_lld.c \
               $(CHIBIOS)/os/hal/ports/SAMA/SAMA5D2x/aic.c
               
# Required include directories.
PLATFORMINC := $(CHIBIOS)/os/hal/ports/SAMA/SAMA5D2x

# Optional platform files.
ifeq ($(USE_SMART_BUILD),yes)

# Configuration files directory
ifeq ($(CONFDIR),)
  CONFDIR = .
endif

HALCONF := $(strip $(shell cat $(CONFDIR)/halconf.h | egrep -e "\#define"))

else
endif

# Drivers compatible with the platform.
include $(CHIBIOS)/os/hal/ports/SAMA/LLD/DMAv1/driver.mk
include $(CHIBIOS)/os/hal/ports/SAMA/LLD/PIOv1/driver.mk
include $(CHIBIOS)/os/hal/ports/SAMA/LLD/SPIv1/driver.mk
include $(CHIBIOS)/os/hal/ports/SAMA/LLD/USARTv1/driver.mk
