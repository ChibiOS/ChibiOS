# List of the ChibiOS generic RP2350 startup and CMSIS files.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt1.c
          
#u can alternatively pick one of these:
#buzzs work
STARTUPASM = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt0_v8m-ml.S \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/vectors.S
# karls work
#STARTUPASM = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt0_v6m.S \
#             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/vectors.S

STARTUPINC = $(CHIBIOS)/os/common/portability/GCC \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC \
             $(CHIBIOS)/os/common/startup/ARMCMx/devices/RP2350 \
             $(CHIBIOS)/os/common/ext/ARM/CMSIS/Core/Include \
             $(CHIBIOS)/os/common/ext/RP/RP2350

STARTUPLD  = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld

# Shared variables
ALLXASMSRC += $(STARTUPASM)
ALLCSRC    += $(STARTUPSRC)
ALLINC     += $(STARTUPINC)
