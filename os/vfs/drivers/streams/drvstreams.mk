# List of all the streams VFS driver files.
DRVSTREAMSSRC := $(CHIBIOS)/os/vfs/drivers/streams/drvstreams.c

# Required include directories
DRVSTREAMSINC := $(CHIBIOS)/os/vfs/drivers/streams

# Shared variables
ALLCSRC += $(DRVSTREAMSSRC)
ALLINC  += $(DRVSTREAMSINC)
