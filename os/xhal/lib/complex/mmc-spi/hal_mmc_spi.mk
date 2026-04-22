# List of all the MMC-SPI subsystem files.
MMCSPISRC := $(CHIBIOS)/os/xhal/lib/complex/mmc-spi/hal_mmc_spi.c

# Required include directories
MMCSPIINC := $(CHIBIOS)/os/xhal/lib/complex/mmc-spi

# Shared variables
ALLCSRC += $(MMCSPISRC)
ALLINC  += $(MMCSPIINC)
