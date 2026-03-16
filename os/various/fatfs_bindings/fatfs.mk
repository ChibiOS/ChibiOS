# FATFS files.
# if needed..
# cd ChibiOS/ext
# 7z x ./littlefs-2.10.1.7z
# 7z x ./fatfs-0.14b_patched.7z
FATFSSRC = $(CHIBIOS)/os/various/fatfs_bindings/fatfs_diskio.c \
           $(CHIBIOS)/os/various/fatfs_bindings/fatfs_syscall.c \
           $(CHIBIOS)/ext/fatfs/source/ff.c \
           $(CHIBIOS)/ext/fatfs/source/ffunicode.c

FATFSINC = $(CHIBIOS)/ext/fatfs/source

# Shared variables
ALLCSRC += $(FATFSSRC)
ALLINC  += $(FATFSINC)
