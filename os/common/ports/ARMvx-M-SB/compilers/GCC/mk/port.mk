# List of the ChibiOS ARMv7/8-M for sandboxes generic port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMvx-M-SB/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMvx-M-SB/compilers/GCC/vectors.S \
          $(CHIBIOS)/os/common/ports/ARMvx-M-SB/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/portability/GCC \
          $(CHIBIOS)/os/common/ports/ARM-common \
          $(CHIBIOS)/os/common/ports/ARMvx-M-SB

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
