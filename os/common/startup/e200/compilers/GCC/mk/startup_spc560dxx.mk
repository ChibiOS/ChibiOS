# List of the ChibiOS e200z0 SPC560Dxx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC560Dxx/boot.s \
             $(CHIBIOS)/os/common/startup/e200/compilers/GCC/vectors.s \
             $(CHIBIOS)/os/common/startup/e200/compilers/GCC/crt0.s

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/GCC \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC560Dxx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/GCC/ld
