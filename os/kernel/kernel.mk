# List of all the ChibiOS/RT kernel files, there is no need to remove the files
# from this list, you can disable parts of the kernel by editing chconf.h.
KERNSRC = ../../os/kernel/src/chsys.c       ../../os/kernel/src/chdebug.c \
          ../../os/kernel/src/chlists.c     ../../os/kernel/src/chvt.c \
          ../../os/kernel/src/chschd.c      ../../os/kernel/src/chthreads.c \
          ../../os/kernel/src/chsem.c       ../../os/kernel/src/chmtx.c \
          ../../os/kernel/src/chcond.c      ../../os/kernel/src/chevents.c \
          ../../os/kernel/src/chmsg.c       ../../os/kernel/src/chmboxes.c \
          ../../os/kernel/src/chqueues.c    ../../os/kernel/src/chheap.c \
          ../../os/kernel/src/chmempools.c

# Required include directories
KERNINC = ../../os/kernel/include
