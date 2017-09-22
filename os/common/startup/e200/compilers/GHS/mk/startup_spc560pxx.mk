# List of the ChibiOS e200z0 SPC560Pxx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC560Pxx/boot.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/GHS/vectors.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/GHS/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/GHS \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC560Pxx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/GHS/ld
