# List of the ChibiOS e200z0 SPC560BCxx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC560BCxx/boot.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/vectors.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC560BCxx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec/ld
