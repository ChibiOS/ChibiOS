# XSNOR + N25Q subsystem build.

# Dependencies.
include $(CHIBIOS)/os/xhal/lib/complex/xsnor/hal_xsnor.mk

# Required files.
MX25SRC := $(CHIBIOS)/os/xhal/lib/complex/xsnor/devices/macronix_mx25/hal_xsnor_macronix_mx25.c

# Required include directories
MX25INC := $(CHIBIOS)/os/xhal/lib/complex/xsnor/include \
           $(CHIBIOS)/os/xhal/lib/complex/xsnor/devices/macronix_mx25

# Shared variables
ALLCSRC += $(MX25SRC)
ALLINC  += $(MX25INC)
