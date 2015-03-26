# List of the ChibiOS/RT Cortex-M0 STM32F0xx port files.
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/startup_stm32f0xx.mk

PORTSRC = $(STARTUPSRC) \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore.c \
          $(CHIBIOS)/os/rt/ports/ARMCMx/chcore_v6m.c
          
PORTASM = $(STARTUPASM) \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/chcoreasm_v6m.s

PORTINC = $(STARTUPINC) \
          $(CHIBIOS)/os/rt/ports/ARMCMx \
          $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC

PORTLD  = $(STARTUPLD)
