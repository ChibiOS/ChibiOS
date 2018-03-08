# List of all the Micron N25Q device files.
M25QSRC := $(CHIBIOS)/os/hal/lib/peripherals/flash/hal_flash.c \
           $(CHIBIOS)/os/hal/lib/peripherals/flash/hal_jesd216_flash.c \
           $(CHIBIOS)/os/hal/lib/complex/m25q/m25q.c \
           $(CHIBIOS)/os/hal/lib/complex/m25q/devices/micron_n25q/m25q_flash.c

# Required include directories
M25QINC := $(CHIBIOS)/os/hal/lib/peripherals/flash \
           $(CHIBIOS)/os/hal/lib/complex/m25q \
           $(CHIBIOS)/os/hal/lib/complex/m25q/devices/micron_n25q

# Shared variables
ALLCSRC += $(M25QSRC)
ALLINC  += $(M25QINC)
