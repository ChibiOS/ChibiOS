# List of all the SPC560B/Cxx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/SPC560BCxx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/SIUL_v1/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/LINFlex_v1/serial_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/SPC560BCxx \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/SIUL_v1 \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/LINFlex_v1
