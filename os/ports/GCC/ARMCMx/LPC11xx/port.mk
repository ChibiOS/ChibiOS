# List of the ChibiOS/RT Cortex-M0 LPC11xx port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore_v6m.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/nvic.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/cmsis/core_cm0.c

PORTASM = ${CHIBIOS}/os/ports/GCC/ARMCMx/crt0.s

PORTINC = ${CHIBIOS}/os/ports/GCC/ARMCMx \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/LPC11xx \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/cmsis
