# List of the ChibiOS e200z4 SPC56ELxx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC56ELxx/boot.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/vectors.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC56ELxx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec/ld
