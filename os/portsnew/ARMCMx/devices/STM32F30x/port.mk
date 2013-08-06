# List of the ChibiOS/RT Cortex-M4 STM32 port files.
PORTSRC = ${CHIBIOS}/os/portsnew/ARMCMx/chcore.c \
          ${CHIBIOS}/os/portsnew/ARMCMx/chcore_v7m.c \
          $(CHIBIOS)/os/portsnew/ARMCMx/GCC/crt0.c \
          $(CHIBIOS)/os/portsnew/ARMCMx/GCC/vectors.c \
          
PORTASM = $(CHIBIOS)/os/portsnew/ARMCMx/GCC/chcoreasm.s

PORTINC = ${CHIBIOS}/os/portsnew/ARMCMx \
          ${CHIBIOS}/os/portsnew/ARMCMx/GCC \
          ${CHIBIOS}/os/portsnew/ARMCMx/common/CMSIS/include \
          ${CHIBIOS}/os/portsnew/ARMCMx/GCC/STM32F30x

PORTLD  = ${CHIBIOS}/os/ports/GCC/ld
