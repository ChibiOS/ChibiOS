# List of the ChibiOS/NIL Cortex-M0 STM32F0xx port files.
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/startup_stm32f0xx.mk

PORTSRC = $(STARTUPSRC) \
          $(CHIBIOS)/os/nil/ports/ARMCMx/nilcore.c \
          $(CHIBIOS)/os/nil/ports/ARMCMx/nilcore_v6m.c
          
PORTASM = $(STARTUPASM) \
          $(CHIBIOS)/os/nil/ports/ARMCMx/compilers/GCC/nilcoreasm_v6m.s

PORTINC = $(STARTUPINC) \
          $(CHIBIOS)/os/nil/ports/ARMCMx \
          $(CHIBIOS)/os/nil/ports/ARMCMx/compilers/GCC

PORTLD  = $(STARTUPLD)
