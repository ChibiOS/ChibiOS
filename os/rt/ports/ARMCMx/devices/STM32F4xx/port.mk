# List of the ChibiOS/RT Cortex-M0 STM32F4xx port files.
PORTSRC = ${CHIBIOS}/os/rt/ports/ARMCMx/chcore.c \
          ${CHIBIOS}/os/rt/ports/ARMCMx/chcore_v7m.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/vectors.c \
          
PORTASM = $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.s

PORTINC = ${CHIBIOS}/os/rt/ports/ARMCMx \
          ${CHIBIOS}/os/rt/ports/ARMCMx/CMSIS/include \
          ${CHIBIOS}/os/rt/ports/ARMCMx/compilers/GCC \
          ${CHIBIOS}/os/rt/ports/ARMCMx/devices/STM32F4xx

PORTLD  = ${CHIBIOS}/os/rt/ports/ARMCMx/compilers/GCC/ld
