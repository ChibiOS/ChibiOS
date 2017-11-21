# List of the ChibiOS generic STM32F4xx startup and CMSIS files.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt1.c \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/vectors.c
          
STARTUPASM = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S

STARTUPINC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC \
             $(CHIBIOS)/os/common/startup/ARMCMx/devices/STM32F4xx \
             $(CHIBIOS)/os/common/ext/ARM/CMSIS/Core/Include \
             $(CHIBIOS)/os/common/ext/ST/STM32F4xx

STARTUPLD  = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld

# Shared variables
ALLASM  += $(STARTUPASM)
ALLCSRC += $(STARTUPSRC)
ALLINC  += $(STARTUPINC)
