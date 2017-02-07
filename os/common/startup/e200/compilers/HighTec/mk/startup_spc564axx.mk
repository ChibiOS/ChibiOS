# List of the ChibiOS e200z4 SPC564Axx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC564Axx/boot.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/vectors.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC564Axx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec/ld
