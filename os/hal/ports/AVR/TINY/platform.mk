# List of all the AVR platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/AVR/TINY/hal_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/AVR/MEGA

# Drivers compatible with the platform.
PLATFORMINC = ${CHIBIOS}/os/hal/ports/AVR/TINY
include ${CHIBIOS}/os/hal/ports/AVR/TINY/GPIOv1/driver.mk
include ${CHIBIOS}/os/hal/ports/AVR/TINY/TIMv1/driver.mk
include ${CHIBIOS}/os/hal/ports/AVR/TINY/USARTv1/driver.mk
