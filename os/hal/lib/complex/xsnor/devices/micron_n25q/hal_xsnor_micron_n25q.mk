# List of all the Micron N25Q device files.
N25QSRC := $(CHIBIOS)/os/hal/lib/complex/xsnor/src/hal_xsnor_base.c \
           $(CHIBIOS)/os/hal/lib/complex/xsnor/devices/micron_n25q/hal_xsnor_micron_n25q.c

# Required include directories
N25QINC := $(CHIBIOS)/os/hal/lib/complex/xsnor/include \
           $(CHIBIOS)/os/hal/lib/complex/xsnor/devices/micron_n25q

# Shared variables
ALLCSRC += $(N25QSRC)
ALLINC  += $(N25QINC)
