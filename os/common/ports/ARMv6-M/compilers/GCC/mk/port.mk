# List of the ChibiOS/RT Cortex-M0 port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMv6-M/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMv6-M/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMv6-M \
          $(CHIBIOS)/os/common/ports/ARMv6-M/compilers/GCC

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)

