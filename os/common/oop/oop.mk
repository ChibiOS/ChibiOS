# List of the ChibiOS common OOP modules.
OOPSRC = ${CHIBIOS}/os/common/oop/src/oop_base_object.c \
         ${CHIBIOS}/os/common/oop/src/oop_referenced_object.c
 
OOPINC = ${CHIBIOS}/os/common/oop/include

# Shared variables
ALLCSRC += $(OOPSRC)
ALLINC  += $(OOPINC)
