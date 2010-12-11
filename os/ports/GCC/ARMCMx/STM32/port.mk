# List of the ChibiOS/RT Cortex-M3 STM32 port files.
PORTSRC = $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32/vectors.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore_v7m.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/nvic.c

PORTASM = ${CHIBIOS}/os/ports/GCC/ARMCMx/crt0_v7m.s

PORTINC = ${CHIBIOS}/os/ports/GCC/ARMCMx \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/STM32
