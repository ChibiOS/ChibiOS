# List of the ChibiOS/RT ARMv8M-mainline generic port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMCMx/chcore.c \
          $(CHIBIOS)/os/common/ports/ARMCMx/chcore_v8m_mainline.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/chcoreasm_v8m-mainline.S

PORTINC = $(CHIBIOS)/os/common/ports/ARMCMx \
          $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC

# Shared variables
ALLXASMSRC += $(PORTASM)
ALLCSRC    += $(PORTSRC)
ALLINC     += $(PORTINC)
