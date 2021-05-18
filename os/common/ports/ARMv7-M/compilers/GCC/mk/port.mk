# List of the ChibiOS/RT ARMv7M generic port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMv7-M/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMv7-M/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMv7-M \
          $(CHIBIOS)/os/common/ports/ARMv7-M/compilers/GCC

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
