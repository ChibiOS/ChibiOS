# Required platform files.
# TODO add $(CHIBIOS)/os/hal/ports/common/ARMCAx/aic.c
PLATFORMSRC := $(CHIBIOS)/os/hal/ports/SAMA/SAMA5D2x/hal_lld.c \
               $(CHIBIOS)/os/hal/ports/SAMA/SAMA5D2x/hal_st_lld.c 
# TODO add $(CHIBIOS)/os/hal/ports/common/ARMCAx     
# Required include directories.
PLATFORMINC := $(CHIBIOS)/os/hal/ports/SAMA/SAMA5D2x

               

# TODO: Add smart build.

# Drivers compatible with the platform.
#include $(CHIBIOS)/os/hal/ports/ATMEL/LLD/DMAv1/driver.mk
#include $(CHIBIOS)/os/hal/ports/ATMEL/LLD/SPIv1/driver.mk
