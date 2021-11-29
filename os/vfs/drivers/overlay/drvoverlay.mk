# List of all the overlay VFS driver files.
DRVOVERLAYSRC := $(CHIBIOS)/os/vfs/drivers/overlay/drvoverlay.c

# Required include directories
DRVOVERLAYINC := $(CHIBIOS)/os/vfs/drivers/overlay

# Shared variables
ALLCSRC += $(DRVOVERLAYSRC)
ALLINC  += $(DRVOVERLAYINC)
