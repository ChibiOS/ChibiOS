# List of the ChibiOS/RT ARM generic port files.

# Dependencies.
include $(CHIBIOS)/os/common/portability/GCC/ccportab.mk
include $(CHIBIOS)/os/common/ports/ARM-common/arm-common.mk

PORTSRC = ${CHIBIOS}/os/common/ports/ARM/chcore.c

PORTASM = $(CHIBIOS)/os/common/ports/ARM/compilers/GCC/chcoreasm.S

PORTINC = ${CHIBIOS}/os/common/ports/ARM \
          ${CHIBIOS}/os/common/ports/ARM/compilers/GCC

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
