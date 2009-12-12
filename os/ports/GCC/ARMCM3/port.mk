# List of the ChibiOS/RT Cortex-M3 port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/ARMCM3/chcore.c \
          ${CHIBIOS}/os/ports/GCC/ARMCM3/nvic.c \
          ${CHIBIOS}/os/ports/GCC/ARMCM3/cmsis/core_cm3.c

PORTASM = ${CHIBIOS}/os/ports/GCC/ARMCM3/crt0.s

PORTINC = ${CHIBIOS}/os/ports/GCC/ARMCM3 \
          ${CHIBIOS}/os/ports/GCC/ARMCM3/cmsis
