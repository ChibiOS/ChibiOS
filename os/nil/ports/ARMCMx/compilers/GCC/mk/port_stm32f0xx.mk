# List of the ChibiOS/NIL Cortex-M0 STM32F0xx port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/vectors.c \
          ${CHIBIOS}/os/nil/ports/ARMCMx/nilcore.c \
          ${CHIBIOS}/os/nil/ports/ARMCMx/nilcore_v6m.c
          
PORTASM = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0_v6m.s \
          $(CHIBIOS)/os/nil/ports/ARMCMx/compilers/GCC/nilcoreasm_v6m.s

PORTINC = ${CHIBIOS}/os/ext/CMSIS/include \
          ${CHIBIOS}/os/ext/CMSIS/ST \
          ${CHIBIOS}/os/common/ports/ARMCMx/devices/STM32F0xx \
          ${CHIBIOS}/os/nil/ports/ARMCMx \
          ${CHIBIOS}/os/nil/ports/ARMCMx/compilers/GCC

PORTLD  = ${CHIBIOS}/os/common/ports/ARMCMx/compilers/GCC/ld
