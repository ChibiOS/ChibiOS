# List of the ChibiOS generic LPC214x file.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/crt1.c

STARTUPASM = $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/vectors.S \
             $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/ARM/devices/SAMA5D2

STARTUPLD  = ${CHIBIOS}/os/common/startup/ARM/compilers/GCC/ld
