# List of all the AVR platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/AVR/XMEGA/ATXMEGAxxxA4U/hal_lld.c \

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/AVR/XMEGA/ATXMEGAxxxA4U

# Drivers compatible with the platform.
include $(CHIBIOS)/os/hal/ports/AVR/XMEGA/LLD/GPIOv1/driver.mk
include $(CHIBIOS)/os/hal/ports/AVR/XMEGA/LLD/TIMv1/driver.mk

# Shared variables
ALLCSRC += $(PLATFORMSRC)
ALLINC  += $(PLATFORMINC)
