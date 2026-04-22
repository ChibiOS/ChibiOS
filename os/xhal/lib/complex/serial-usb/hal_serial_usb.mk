# List of all the Serial USB subsystem files.
SERIALUSBSRC := $(CHIBIOS)/os/xhal/lib/complex/serial-usb/hal_serial_usb.c

# Required include directories
SERIALUSBINC := $(CHIBIOS)/os/xhal/lib/complex/serial-usb

# Shared variables
ALLCSRC += $(SERIALUSBSRC)
ALLINC  += $(SERIALUSBINC)
