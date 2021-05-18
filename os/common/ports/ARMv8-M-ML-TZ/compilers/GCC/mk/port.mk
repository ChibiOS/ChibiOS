# List of the ChibiOS/RT ARMv8-M mainline generic port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMv8-M-ML-TZ/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMv8-M-ML-TZ/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMv8-M-ML-TZ \
          $(CHIBIOS)/os/common/ports/ARMv8-M-ML-TZ/compilers/GCC

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
