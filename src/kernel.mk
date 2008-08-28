# List of all the ChibiOS/RT kernel files, there is no need to remove the files
# from this list, you can disable parts of the kernel by editing chconf.h.
KERNSRC = ../../src/chinit.c   ../../src/chdebug.c \
          ../../src/chlists.c  ../../src/chdelta.c \
          ../../src/chschd.c   ../../src/chthreads.c \
          ../../src/chsem.c    ../../src/chmtx.c \
          ../../src/chevents.c ../../src/chmsg.c \
          ../../src/chsleep.c  ../../src/chqueues.c \
          ../../src/chserial.c ../../src/chheap.c \
          ../../src/chmempools.c
