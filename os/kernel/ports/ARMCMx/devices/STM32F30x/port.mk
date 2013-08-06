# List of the ChibiOS/RT Cortex-M4 STM32F30x port files.
PORTSRC = ${CHIBIOS}/os/kernel/ports/ARMCMx/chcore.c \
          ${CHIBIOS}/os/kernel/ports/ARMCMx/chcore_v7m.c \
          $(CHIBIOS)/os/kernel/ports/ARMCMx/GCC/crt0.c \
          $(CHIBIOS)/os/kernel/ports/ARMCMx/GCC/vectors.c \
          
PORTASM = $(CHIBIOS)/os/kernel/ports/ARMCMx/GCC/chcoreasm_v7m.s

PORTINC = ${CHIBIOS}/os/kernel/ports/ARMCMx \
          ${CHIBIOS}/os/kernel/ports/ARMCMx/GCC \
          ${CHIBIOS}/os/kernel/ports/ARMCMx/GCC/STM32F30x \
          ${CHIBIOS}/os/kernel/ports/ARMCMx/common/CMSIS/include

PORTLD  = ${CHIBIOS}/os/ports/GCC/ld
