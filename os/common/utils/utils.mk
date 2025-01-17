# Utils subsystem build.

# Required files.
ifndef UTILSSRC
  ifndef UTILSSELECT
    # If none is specified then all of them.
    UTILSSRC = ${CHIBIOS}/os/common/utils/src/paths.c \
               ${CHIBIOS}/os/common/utils/src/sglob.c
  else
    ifneq ($(filter paths,$(UTILSSELECT)),)
      UTILSSRC += ${CHIBIOS}/os/common/utils/src/paths.c
    endif
    ifneq ($(filter sglob,$(UTILSSELECT)),)
      UTILSSRC += ${CHIBIOS}/os/common/utils/src/sglob.c
    endif
  endif
  ALLCSRC += $(UTILSSRC)
endif
 
# Required include directories
ifndef UTILSINC
  UTILSINC = ${CHIBIOS}/os/common/utils/include
  ALLINC += $(UTILSINC)
endif
