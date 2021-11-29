# List of all the FatFS VFS driver files.
DRVFATFSSRC := $(CHIBIOS)/os/vfs/drivers/fatfs/drvfatfs.c

# Required include directories
DRVFATFSINC := $(CHIBIOS)/os/vfs/drivers/fatfs

# Shared variables
ALLCSRC += $(DRVFATFSSRC)
ALLINC  += $(DRVFATFSINC)
