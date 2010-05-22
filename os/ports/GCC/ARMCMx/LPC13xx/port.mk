# List of the ChibiOS/RT Cortex-M0 LPC13xx port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore_v7m.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/nvic.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/cmsis/core_cm3.c

PORTASM = ${CHIBIOS}/os/ports/GCC/ARMCMx/crt0.s

PORTINC = ${CHIBIOS}/os/ports/GCC/ARMCMx \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/LPC13xx \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/cmsis
