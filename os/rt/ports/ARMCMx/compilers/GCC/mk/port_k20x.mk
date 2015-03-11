# List of the ChibiOS/RT Cortex-M4 K20x port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/vectors.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore_v7m.c

PORTASM = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0_v7m.s \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.s

PORTINC = $(CHIBIOS)/os/ext/CMSIS/include \
          $(CHIBIOS)/os/ext/CMSIS/KINETIS \
          $(CHIBIOS)/os/common/ports/ARMCMx/devices/K20x \
          $(CHIBIOS)/os/rt/ports/ARMCMx \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC

PORTLD  = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/ld
