# List of the ChibiOS generic SAMA5D2 file.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/crt1.c

STARTUPASM = $(CHIBIOS)/os/common/startup/ARM/devices/SAMA5D2/boot.S \
             $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/vectors.S \
             $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/ARM/devices/SAMA5D2 \
             $(CHIBIOS)/os/common/ext/ARM/CMSIS/Core_A/Include 

STARTUPLD  = ${CHIBIOS}/os/common/startup/ARM/compilers/GCC/ld
