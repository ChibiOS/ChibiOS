# List of the ChibiOS/NIL Cortex-M4 STM32F37x port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/vectors.c \
          ${CHIBIOS}/os/nil/ports/ARMCMx/nilcore.c \
          ${CHIBIOS}/os/nil/ports/ARMCMx/nilcore_v7m.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0_v7m.s \
          $(CHIBIOS)/os/nil/ports/ARMCMx/compilers/GCC/nilcoreasm_v7m.s

PORTINC = ${CHIBIOS}/os/ext/CMSIS/include \
          ${CHIBIOS}/os/ext/CMSIS/ST \
          ${CHIBIOS}/os/common/ports/ARMCMx/devices/STM32F3xx \
          ${CHIBIOS}/os/nil/ports/ARMCMx \
          ${CHIBIOS}/os/nil/ports/ARMCMx/compilers/GCC

PORTLD  = ${CHIBIOS}/os/common/ports/ARMCMx/compilers/GCC/ld
