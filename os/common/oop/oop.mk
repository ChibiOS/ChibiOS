# OOP subsystem build.

# Required files.
ifndef OOPSRC
  ifndef OOPSELECT
    # If none is specified then all of them.
    OOPSRC = ${CHIBIOS}/os/common/oop/src/oop_base_object.c \
             ${CHIBIOS}/os/common/oop/src/oop_referenced_object.c \
             ${CHIBIOS}/os/common/oop/src/oop_synchronized_object.c
  else
    ifneq ($(filter base,$(OOPSELECT)),)
      OOPSRC += ${CHIBIOS}/os/common/oop/src/oop_base_object.c
    endif
    ifneq ($(filter referenced,$(OOPSELECT)),)
      OOPSRC += ${CHIBIOS}/os/common/oop/src/oop_referenced_object.c
    endif
    ifneq ($(filter synchronized,$(OOPSELECT)),)
      OOPSRC += ${CHIBIOS}/os/common/oop/src/oop_synchronized_object.c
    endif
  endif
  ALLCSRC += $(OOPSRC)
endif

# Required include directories
ifndef OOPINC
  OOPINC = ${CHIBIOS}/os/common/oop/include
  ALLINC += $(OOPINC)
endif
