# FATFS files.
FATFSSRC = ${CHIBIOS}/os/various/fatfs_bindings/fatfs_diskio.c \
           ${CHIBIOS}/os/various/fatfs_bindings/fatfs_syscall.c \
           ${CHIBIOS}/os/ext/fatfs/src/ff.c \
           ${CHIBIOS}/os/ext/fatfs/src/option/unicode.c

FATFSINC = ${CHIBIOS}/os/ext/fatfs/src
