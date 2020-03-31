# List of the ChibiOS/RT ARMv8-M mainline generic port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMv8-M-ML/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMv8-M-ML/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMv8-M-ML \
          $(CHIBIOS)/os/common/ports/ARMv8-M-ML/compilers/GCC

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
