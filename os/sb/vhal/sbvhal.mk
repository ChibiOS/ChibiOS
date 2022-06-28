# List of the ChibiOS sandbox VHAL files.
SBVHALSRC = $(CHIBIOS)/os/sb/vhal/sbvhal.c \
            $(CHIBIOS)/os/sb/vhal/sbvhal_pal.c
 
SBVHALASM =

SBVHALINC = $(CHIBIOS)/os/sb/vhal

# Shared variables
ALLXASMSRC += $(SBVHALASM)
ALLCSRC    += $(SBVHALSRC)
ALLINC     += $(SBVHALINC)
