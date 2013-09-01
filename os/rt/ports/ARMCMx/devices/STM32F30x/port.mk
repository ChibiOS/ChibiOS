# List of the ChibiOS/RT Cortex-M4 STM32F30x port files.
PORTSRC = ${CHIBIOS}/os/rt/ports/ARMCMx/chcore.c \
          ${CHIBIOS}/os/rt/ports/ARMCMx/chcore_v7m.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/vectors.c \
          
PORTASM = $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.s

PORTINC = ${CHIBIOS}/os/ext/CMSIS/include \
          ${CHIBIOS}/os/rt/ports/ARMCMx \
          ${CHIBIOS}/os/rt/ports/ARMCMx/compilers/GCC \
          ${CHIBIOS}/os/rt/ports/ARMCMx/devices/STM32F30x

PORTLD  = ${CHIBIOS}/os/rt/ports/ARMCMx/compilers/GCC/ld
