XSNOR driver, usage and makefile inclusion notes.

XSNOR driver uses the HAL WSPI driver for I/O, SPI is not directly supported
unless somehow hidden behind a WSPI API (to-do for STM32).

This driver is compatible with the previous serial_nor driver except for the
initialization function which is now named xxxObjectInit() where xxx is the
device prefix (n25q for example).

Other functions previously prefixed with "snor" are now prefixed with "xsnor"
in order to avoid conflicts (you can use both in the same application).

For Micron N25Q add the following:

include $(CHIBIOS)/os/common/oop/oop.mk
include $(CHIBIOS)/os/hal/lib/complex/snor/devices/micron_n25q/hal_snor_micron_n25q.mk

