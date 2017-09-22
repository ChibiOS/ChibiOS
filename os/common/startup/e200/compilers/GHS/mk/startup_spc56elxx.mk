# List of the ChibiOS e200z4 SPC56ELxx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC56ELxx/boot.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/GHS/vectors.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/GHS/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/GHS \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC56ELxx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/GHS/ld
