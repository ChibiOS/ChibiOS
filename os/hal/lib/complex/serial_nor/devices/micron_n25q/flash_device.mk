# List of all the Micron N25Q device files.
N25QSRC := $(CHIBIOS)/os/hal/lib/peripherals/flash/hal_flash.c \
           $(CHIBIOS)/os/hal/lib/complex/serial_nor/serial_nor.c \
           $(CHIBIOS)/os/hal/lib/complex/serial_nor/devices/micron_n25q/flash_device.c

# Required include directories
N25QINC := $(CHIBIOS)/os/hal/lib/peripherals/flash \
           $(CHIBIOS)/os/hal/lib/complex/serial_nor \
           $(CHIBIOS)/os/hal/lib/complex/serial_nor/devices/micron_n25q

# Shared variables
ALLCSRC += $(N25QSRC)
ALLINC  += $(N25QINC)
