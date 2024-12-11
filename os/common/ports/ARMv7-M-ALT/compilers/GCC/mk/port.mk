# List of the ChibiOS/RT ARMv7M generic port files.

# Dependencies.
include $(CHIBIOS)/os/common/portability/GCC/ccportab.mk
include $(CHIBIOS)/os/common/ports/ARM-common/arm-common.mk

PORTSRC = $(CHIBIOS)/os/common/ports/ARMv7-M-ALT/chcore.c

PORTASM = $(CHIBIOS)/os/common/ports/ARMv7-M-ALT/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMv7-M-ALT

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
