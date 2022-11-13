# List of all the MMC_SPI subsystem files.
MMCSDSPISRC := $(CHIBIOS)/os/hal/lib/complex/mmcsd_spi/hal_mmcsd_spi.c

# Required include directories
MMCSDSPIINC := $(CHIBIOS)/os/hal/lib/complex/mmcsd_spi

# Shared variables
ALLCSRC += $(MMCSDSPISRC)
ALLINC  += $(MMCSDSPIINC)
