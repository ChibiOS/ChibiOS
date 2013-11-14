# List of all the ChibiOS/RT kernel files, there is no need to remove the files
# from this list, you can disable parts of the kernel by editing chconf.h.
KERNSRC = ${CHIBIOS}/os/rt/src/chsys.c \
          ${CHIBIOS}/os/rt/src/chdebug.c \
          ${CHIBIOS}/os/rt/src/chtm.c \
          ${CHIBIOS}/os/rt/src/chstats.c \
          ${CHIBIOS}/os/rt/src/chschd.c \
          ${CHIBIOS}/os/rt/src/chvt.c \
          ${CHIBIOS}/os/rt/src/chthreads.c \
          ${CHIBIOS}/os/rt/src/chdynamic.c \
          ${CHIBIOS}/os/rt/src/chregistry.c \
          ${CHIBIOS}/os/rt/src/chsem.c \
          ${CHIBIOS}/os/rt/src/chmtx.c \
          ${CHIBIOS}/os/rt/src/chcond.c \
          ${CHIBIOS}/os/rt/src/chevents.c \
          ${CHIBIOS}/os/rt/src/chmsg.c \
          ${CHIBIOS}/os/rt/src/chmboxes.c \
          ${CHIBIOS}/os/rt/src/chqueues.c \
          ${CHIBIOS}/os/rt/src/chmemcore.c \
          ${CHIBIOS}/os/rt/src/chheap.c \
          ${CHIBIOS}/os/rt/src/chmempools.c

# Required include directories
KERNINC = ${CHIBIOS}/os/rt/include
