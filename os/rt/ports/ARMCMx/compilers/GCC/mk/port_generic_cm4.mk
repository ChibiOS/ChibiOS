# List of the ChibiOS/RT Cortex-M4 generic port files.
PORTSRC = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/crt0.c \
          $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/vectors.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore_v7m.c
          
PORTASM = $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.s

PORTINC = $(CHIBIOS)/os/ext/CMSIS/include \
          $(CHIBIOS)/os/rt/ports/ARMCMx \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC
