# List of the ChibiOS e200z3 SPC563Mxx startup files.
STARTUPSRC =
          
STARTUPASM = $(CHIBIOS)/os/common/startup/e200/devices/SPC563Mxx/boot.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/vectors.S \
             $(CHIBIOS)/os/common/startup/e200/compilers/HighTec/crt0.S

STARTUPINC = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec \
             ${CHIBIOS}/os/common/startup/e200/devices/SPC563Mxx

STARTUPLD  = ${CHIBIOS}/os/common/startup/e200/compilers/HighTec/ld
