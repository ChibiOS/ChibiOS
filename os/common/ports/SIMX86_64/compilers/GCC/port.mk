# List of the ChibiOS/RT SIMX86_64 port files.

# Dependencies.
include $(CHIBIOS)/os/common/portability/GCC/ccportab.mk

PORTSRC = ${CHIBIOS}/os/common/ports/SIMX86_64/chcore.c

PORTASM = 

PORTINC = ${CHIBIOS}/os/common/ports/SIMX86_64/compilers/GCC \
          ${CHIBIOS}/os/common/ports/SIMX86_64

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
