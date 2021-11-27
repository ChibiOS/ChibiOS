# List of all the FatFS VFS driver files.
DRVFATFSSSRC := $(CHIBIOS)/os/vfs/drivers/fatfs/drvfatfs.c

# Required include directories
DRVFATFSSINC := $(CHIBIOS)/os/vfs/drivers/fatfs

# Shared variables
ALLCSRC += $(DRVFATFSSSRC)
ALLINC  += $(DRVFATFSSINC)
