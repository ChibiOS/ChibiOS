# List of the ChibiOS/RT Cortex-M3 STM32 port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore_v7m.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/nvic.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/cmsis/core_cm3.c

PORTASM = ${CHIBIOS}/os/ports/GCC/ARMCMx/crt0_v7m.s

PORTINC = ${CHIBIOS}/os/ports/GCC/ARMCMx \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/STM32 \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/cmsis
