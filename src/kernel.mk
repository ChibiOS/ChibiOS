# List of all the ChibiOS/RT kernel files, there is no need to remove the files
# from this list, you can disable parts of the kernel by editing chconf.h.
KERNSRC = ../../src/chsys.c    ../../src/chdebug.c \
          ../../src/chlists.c  ../../src/chvt.c \
          ../../src/chschd.c   ../../src/chthreads.c \
          ../../src/chsem.c    ../../src/chmtx.c \
          ../../src/chcond.c   ../../src/chevents.c \
          ../../src/chmsg.c    ../../src/chqueues.c \
          ../../src/chheap.c   ../../src/chmempools.c \
          ../../src/chserial.c
