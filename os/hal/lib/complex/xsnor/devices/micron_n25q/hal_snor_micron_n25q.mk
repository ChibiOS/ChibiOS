# List of all the Micron N25Q device files.
N25QSRC := $(CHIBIOS)/os/hal/lib/complex/snor/src/hal_snor_base.c \
           $(CHIBIOS)/os/hal/lib/complex/snor/devices/micron_n25q/hal_snor_micron_n25q.c

# Required include directories
N25QINC := $(CHIBIOS)/os/hal/lib/complex/snor/include \
           $(CHIBIOS)/os/hal/lib/complex/snor/devices/micron_n25q

# Shared variables
ALLCSRC += $(N25QSRC)
ALLINC  += $(N25QINC)
