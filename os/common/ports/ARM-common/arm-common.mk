# ARM-common subsystem build.

# Required files.
ifndef ARMCOMMONSRC
  ARMCOMMONSRC =
  ALLCSRC += $(ARMCOMMONSRC)
endif

# Required include directories
ifndef ARMCOMMONINC
  ARMCOMMONINC = ${CHIBIOS}/os/common/ports/ARM-common/include
  ALLINC += $(ARMCOMMONINC)
endif
