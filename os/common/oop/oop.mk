# OOP subsystem build.

# Required files.
ifeq ($(OOPSRC),)
  OOPSRC = ${CHIBIOS}/os/common/oop/src/oop_base_object.c \
           ${CHIBIOS}/os/common/oop/src/oop_referenced_object.c
  ALLCSRC += $(OOPSRC)
endif

# Required include directories
ifeq ($(OOPINC),)
  OOPINC = ${CHIBIOS}/os/common/oop/include
  ALLINC += $(OOPINC)
endif
