# List of the ChibiOS/NIL Cortex-M3 STM32F1xx port files.
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/startup_stm32f1xx.mk

PORTSRC = $(STARTUPSRC) \
          $(CHIBIOS)/os/nil/ports/ARMCMx/nilcore.c \
          $(CHIBIOS)/os/nil/ports/ARMCMx/nilcore_v7m.c
          
PORTASM = $(STARTUPASM) \
          $(CHIBIOS)/os/nil/ports/ARMCMx/compilers/GCC/nilcoreasm_v7m.s

PORTINC = $(STARTUPINC) \
          $(CHIBIOS)/os/nil/ports/ARMCMx \
          $(CHIBIOS)/os/nil/ports/ARMCMx/compilers/GCC

PORTLD  = $(STARTUPLD)
