# List of all the Macronix MX25 device files.
MX25SRC := $(CHIBIOS)/os/hal/lib/complex/snor/src/hal_snor_base.c \
           $(CHIBIOS)/os/hal/lib/complex/snor/devices//macronix_mx25/hal_snor_macronix_mx25.c

# Required include directories
MX25INC := $(CHIBIOS)/os/hal/lib/complex/snor/include \
           $(CHIBIOS)/os/hal/lib/complex/snor/devices/macronix_mx25

# Shared variables
ALLCSRC += $(MX25SRC)
ALLINC  += $(MX25INC)
