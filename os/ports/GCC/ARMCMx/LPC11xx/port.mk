# List of the ChibiOS/RT Cortex-M0 LPC11xx port files.
PORTSRC = $(CHIBIOS)/os/ports/GCC/ARMCMx/LPC11xx/vectors.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/chcore_v6m.c \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/nvic.c

PORTASM = ${CHIBIOS}/os/ports/GCC/ARMCMx/crt0_v6m.s

PORTINC = ${CHIBIOS}/os/ports/GCC/ARMCMx \
          ${CHIBIOS}/os/ports/GCC/ARMCMx/LPC11xx
