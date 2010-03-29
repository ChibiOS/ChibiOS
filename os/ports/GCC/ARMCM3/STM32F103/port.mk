# List of the ChibiOS/RT Cortex-M3 port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/nvic.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/cmsis/core_cm3.c

PORTASM = ${CHIBIOS}/os/ports/GCC/ARMCMx/crt0.s

PORTINC = ${CHIBIOS}/os/ports/GCC/ARMCMx \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/STM32F103 \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/cmsis
