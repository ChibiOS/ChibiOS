# List of the ChibiOS e200z4 SPC56ECxx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC56ECxx/boot.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/GHS/vectors.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/GHS/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/GHS \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC56ECxx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/GHS/ld
