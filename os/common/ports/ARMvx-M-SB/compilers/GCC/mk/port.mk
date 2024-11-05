# List of the ChibiOS ARMv7/8-M for sandboxes generic port files.

# Dependencies.
include $(CHIBIOS)/os/common/portability/GCC/ccportab.mk
include $(CHIBIOS)/os/common/ports/ARM-common/arm-common.mk

PORTSRC = $(CHIBIOS)/os/common/ports/ARMvx-M-SB/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMvx-M-SB/compilers/GCC/vectors.S \
          $(CHIBIOS)/os/common/ports/ARMvx-M-SB/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMvx-M-SB

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
