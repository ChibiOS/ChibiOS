# XSNOR + N25Q subsystem build.

# Dependencies.
include $(CHIBIOS)/os/xhal/lib/complex/xsnor/hal_xsnor.mk

# Required files.
N25QSRC := $(CHIBIOS)/os/xhal/lib/complex/xsnor/devices/micron_n25q/hal_xsnor_micron_n25q.c

# Required include directories
N25QINC := $(CHIBIOS)/os/xhal/lib/complex/xsnor/devices/micron_n25q

# Shared variables
ALLCSRC += $(N25QSRC)
ALLINC  += $(N25QINC)
