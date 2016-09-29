# List of the ChibiOS generic XMC1100 startup and CMSIS files.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt1.c
          
STARTUPASM = $(CHIBIOS)/os/common/startup/ARMCMx/devices/XMC1100/veneers_gcc.S \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt0_v6m.S

STARTUPINC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC \
             $(CHIBIOS)/os/common/startup/ARMCMx/devices/XMC1100 \
             $(CHIBIOS)/os/common/ext/CMSIS/include \
             $(CHIBIOS)/os/common/ext/CMSIS/Infineon/XMC1100

STARTUPLD  = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld
