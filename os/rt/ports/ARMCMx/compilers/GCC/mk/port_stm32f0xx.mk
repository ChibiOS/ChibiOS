# List of the ChibiOS/RT Cortex-M0 STM32F0xx port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/vectors.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore_v6m.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0_v6m.s \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v6m.s

PORTINC = $(CHIBIOS)/os/ext/CMSIS/include \
          $(CHIBIOS)/os/ext/CMSIS/ST \
          $(CHIBIOS)/os/common/ports/ARMCMx/devices/STM32F0xx \
          $(CHIBIOS)/os/rt/ports/ARMCMx \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC

PORTLD  = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/ld
