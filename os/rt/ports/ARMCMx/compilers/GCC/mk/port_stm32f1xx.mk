# List of the ChibiOS/RT Cortex-M3 STM32F1xx port files.
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/startup_stm32f1xx.mk

PORTSRC = $(STARTUPSRC) \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore_v7m.c
          
PORTASM = $(STARTUPASM) \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.s

PORTINC = $(STARTUPINC) \
          $(CHIBIOS)/os/rt/ports/ARMCMx \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC

PORTLD  = $(STARTUPLD)
