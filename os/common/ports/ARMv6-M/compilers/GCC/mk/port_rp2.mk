# List of the ChibiOS/RT Cortex-M0 port files.

# Dependencies.
include $(CHIBIOS)/os/common/portability/GCC/ccportab.mk
include $(CHIBIOS)/os/common/ports/ARM-common/arm-common.mk

PORTSRC = $(CHIBIOS)/os/common/ports/ARMv6-M/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMv6-M/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMv6-M \
          $(CHIBIOS)/os/common/ports/ARMv6-M\smp\rp2

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)

