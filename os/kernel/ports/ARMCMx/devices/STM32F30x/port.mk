# List of the ChibiOS/RT Cortex-M4 STM32F30x port files.
PORTSRC = ${CHIBIOS}/os/kernel/ports/ARMCMx/chcore.c \
          ${CHIBIOS}/os/kernel/ports/ARMCMx/chcore_v7m.c \
          $(CHIBIOS)/os/kernel/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/kernel/ports/ARMCMx/compilers/GCC/vectors.c \
          
PORTASM = $(CHIBIOS)/os/kernel/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.s

PORTINC = ${CHIBIOS}/os/kernel/ports/ARMCMx \
          ${CHIBIOS}/os/kernel/ports/ARMCMx/CMSIS/include \
          ${CHIBIOS}/os/kernel/ports/ARMCMx/compilers/GCC \
          ${CHIBIOS}/os/kernel/ports/ARMCMx/devices/STM32F30x

PORTLD  = ${CHIBIOS}/os/kernel/ports/ARMCMx/compilers/GCC/ld
