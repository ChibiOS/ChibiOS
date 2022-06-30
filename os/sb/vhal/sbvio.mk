# List of the ChibiOS sandbox VHAL files.
SBVHALSRC = $(CHIBIOS)/os/sb/vhal/sbvio.c \
            $(CHIBIOS)/os/sb/vhal/sbvio_gpio.c
 
SBVHALASM =

SBVHALINC = $(CHIBIOS)/os/sb/vhal

# Shared variables
ALLXASMSRC += $(SBVHALASM)
ALLCSRC    += $(SBVHALSRC)
ALLINC     += $(SBVHALINC)
