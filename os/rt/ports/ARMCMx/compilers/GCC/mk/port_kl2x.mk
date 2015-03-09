# List of the ChibiOS/RT Cortex-M0+ KL2x port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/vectors.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore_v6m.c

PORTASM = $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v6m.s

PORTINC = $(CHIBIOS)/os/ext/CMSIS/include \
          $(CHIBIOS)/os/ext/CMSIS/KINETIS \
          $(CHIBIOS)/os/common/ports/ARMCMx/devices/KL2x \
          $(CHIBIOS)/os/rt/ports/ARMCMx \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC

PORTLD  = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/ld
