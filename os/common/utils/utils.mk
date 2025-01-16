# Utils subsystem build.

# Required files.
ifndef UTILSSRC
  UTILSSRC = ${CHIBIOS}/os/common/utils/src/paths.c
  ALLCSRC += $(UTILSSRC)
endif
 
# Required include directories
ifndef UTILSINC
  UTILSINC = ${CHIBIOS}/os/common/utils/include
  ALLINC += $(UTILSINC)
endif
