# List of all the m25Q device files.
M25QSRC := $(CHIBIOS)/os/hal/lib/peripherals/flash/hal_flash.c \
           $(CHIBIOS)/os/hal/lib/peripherals/flash/hal_jesd216_flash.c \
           $(CHIBIOS)/os/ex/Micron/m25q.c

# Required include directories
M25QINC := $(CHIBIOS)/os/hal/lib/peripherals/flash \
           $(CHIBIOS)/os/ex/Micron

# Shared variables
ALLCSRC += $(M25QSRC)
ALLINC  += $(M25QINC)
