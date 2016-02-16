# List of the ChibiOS generic LPC214x file.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/crt1.c

STARTUPASM = $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/vectors.s \
             $(CHIBIOS)/os/common/startup/ARM/compilers/GCC/crt0.s

STARTUPINC = ${CHIBIOS}/os/common/startup/ARM/devices/LPC214x

STARTUPLD  = ${CHIBIOS}/os/common/startup/ARM/compilers/GCC/ld
