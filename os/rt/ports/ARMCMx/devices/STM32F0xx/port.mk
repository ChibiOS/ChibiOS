# List of the ChibiOS/RT Cortex-M0 STM32F0xx port files.
PORTSRC = ${CHIBIOS}/os/rt/ports/ARMCMx/chcore.c \
          ${CHIBIOS}/os/rt/ports/ARMCMx/chcore_v6m.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/vectors.c \
          
PORTASM = $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v6m.s

PORTINC = ${CHIBIOS}/os/ext/CMSIS/include \
          ${CHIBIOS}/os/ext/CMSIS/ST \
          ${CHIBIOS}/os/rt/ports/ARMCMx \
          ${CHIBIOS}/os/rt/ports/ARMCMx/compilers/GCC \
          ${CHIBIOS}/os/rt/ports/ARMCMx/devices/STM32F0xx

PORTLD  = ${CHIBIOS}/os/rt/ports/ARMCMx/compilers/GCC/ld
