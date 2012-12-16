# List of all the SPC56x platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/SPC563Mxx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/ESCI_v1/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/SIU_v1/pal_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/SPC563Mxx \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/ESCI_v1 \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/SIU_v1
