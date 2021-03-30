# List of the ChibiOS/RT RP2 port files, note it uses files from the
# normal ARMv6-M port.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMv6-M-RP2/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMv6-M/compilers/GCC/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMv6-M-RP2 \
          $(CHIBIOS)/os/common/ports/ARMv6-M/compilers/GCC

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)

