# List of the ChibiOS generic STM32H5xx startup and CMSIS files.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt1.c
          
STARTUPASM = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt0_v8m-ml.S \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/vectors.S

STARTUPINC = $(CHIBIOS)/os/common/portability/GCC \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC \
             $(CHIBIOS)/os/common/startup/ARMCMx/devices/STM32H5xx \
             $(CHIBIOS)/os/common/ext/ARM/CMSIS/Core/Include \
             $(CHIBIOS)/os/common/ext/ST/STM32H5xx

STARTUPLD  = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld

# Shared variables
ALLXASMSRC += $(STARTUPASM)
ALLCSRC    += $(STARTUPSRC)
ALLINC     += $(STARTUPINC)
