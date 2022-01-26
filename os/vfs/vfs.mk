# List of all the ChibiOS/VFS files.
VFSSRC := $(CHIBIOS)/os/vfs/src/vfsparser.c \
          $(CHIBIOS)/os/vfs/src/vfsbuffers.c \
          $(CHIBIOS)/os/vfs/src/vfsdrivers.c \
          $(CHIBIOS)/os/vfs/src/vfs.c \
          $(CHIBIOS)/os/vfs/drivers/template/drvtemplate.c \
          $(CHIBIOS)/os/vfs/drivers/sfs/drvsfs.c \
          $(CHIBIOS)/os/vfs/drivers/fatfs/drvfatfs.c \
          $(CHIBIOS)/os/vfs/drivers/overlay/drvoverlay.c \
          $(CHIBIOS)/os/vfs/drivers/streams/drvstreams.c

# Required include directories
VFSINC := $(CHIBIOS)/os/vfs/include \
          $(CHIBIOS)/os/vfs/drivers/template \
          $(CHIBIOS)/os/vfs/drivers/sfs \
          $(CHIBIOS)/os/vfs/drivers/fatfs \
          $(CHIBIOS)/os/vfs/drivers/overlay \
          $(CHIBIOS)/os/vfs/drivers/streams

# Shared variables
ALLCSRC += $(VFSSRC)
ALLINC  += $(VFSINC)
