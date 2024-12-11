# List of the ChibiOS/RT ARMv8-M mainline generic port files.

# Dependencies.
include $(CHIBIOS)/os/common/portability/GCC/ccportab.mk
include $(CHIBIOS)/os/common/ports/ARM-common/arm-common.mk

PORTSRC = $(CHIBIOS)/os/common/ports/ARMv8-M-ML-ALT/chcore.c

PORTASM = $(CHIBIOS)/os/common/ports/ARMv8-M-ML-ALT/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMv8-M-ML-ALT

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
