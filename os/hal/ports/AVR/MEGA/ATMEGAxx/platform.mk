# List of all the AVR platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_lld.c \

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/AVR/MEGA

# Drivers compatible with the platform.
include $(CHIBIOS)/os/hal/ports/AVR/MEGA/ADCv1/driver.mk
include $(CHIBIOS)/os/hal/ports/AVR/MEGA/EXTv1/driver.mk
include $(CHIBIOS)/os/hal/ports/AVR/MEGA/GPIOv1/driver.mk
include $(CHIBIOS)/os/hal/ports/AVR/MEGA/I2Cv1/driver.mk
include $(CHIBIOS)/os/hal/ports/AVR/MEGA/SPIv1/driver.mk
include $(CHIBIOS)/os/hal/ports/AVR/MEGA/TIMv1/driver.mk
include $(CHIBIOS)/os/hal/ports/AVR/MEGA/USARTv1/driver.mk
include $(CHIBIOS)/os/hal/ports/AVR/MEGA/USBv1/driver.mk
