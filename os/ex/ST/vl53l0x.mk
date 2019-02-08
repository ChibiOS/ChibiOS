# List of all the VL53L0X device files.
VL53L0XSRC := $(CHIBIOS)/os/ex/ST/vl53l0x.c

# Required include directories
VL53L0XINC := $(CHIBIOS)/os/hal/lib/peripherals/sensors \
              $(CHIBIOS)/os/ex/ST

# Shared variables
ALLCSRC += $(VL53L0XSRC)
ALLINC  += $(VL53L0XINC)