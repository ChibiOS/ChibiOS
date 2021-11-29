# List of all the ChibiOS/VFS files.
VFSSRC := $(CHIBIOS)/os/vfs/src/vfsparser.c \
          $(CHIBIOS)/os/vfs/src/vfs.c

# Required include directories
VFSINC := $(CHIBIOS)/os/vfs/include

# Shared variables
ALLCSRC += $(VFSSRC)
ALLINC  += $(VFSINC)
