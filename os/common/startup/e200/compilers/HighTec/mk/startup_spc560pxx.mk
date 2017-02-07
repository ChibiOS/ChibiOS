# List of the ChibiOS e200z0 SPC560Pxx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC560Pxx/boot.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/vectors.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC560Pxx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec/ld
