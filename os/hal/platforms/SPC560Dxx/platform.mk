# List of all the SPC560Dxx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/SPC560Dxx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/EDMA_v1/spc5_edma.c \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/SIUL_v1/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/LINFlex_v1/serial_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/SPC560Dxx \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/EDMA_v1 \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/SIUL_v1 \
              ${CHIBIOS}/os/hal/platforms/SPC5xx/LINFlex_v1
