# List of the ChibiOS/RT AVR port files.

# Dependencies.
include $(CHIBIOS)/os/common/portability/GCC/ccportab.mk

PORTSRC = ${CHIBIOS}/os/common/ports/AVR/chcore.c

PORTASM = 

PORTINC = ${CHIBIOS}/os/common/ports/AVR \
          ${CHIBIOS}/os/common/ports/AVR/compilers/GCC

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
