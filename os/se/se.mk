# List of all the ChibiOS/SE kernel file.
SESRC := $(CHIBIOS)/os/se/src/seloops.c

# Required include directories
SEINC := $(CHIBIOS)/os/se/include

# Shared variables
ALLCSRC += $(SESRC)
ALLINC  += $(SEINC)
