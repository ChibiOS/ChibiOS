# FATFS files.
FATFSSRC = $(CHIBIOS)/os/various/syscalls.c \
		   $(CHIBIOS)/os/various/fatfs_bindings/SAMA5D2x/ch_sdmmc_ff.c \
           $(CHIBIOS)/os/various/fatfs_bindings/fatfs_syscall.c \
           $(CHIBIOS)/ext/fatfs/src/ff.c \
           $(CHIBIOS)/ext/fatfs/src/ffunicode.c
		   
FATFSINC = $(CHIBIOS)/ext/fatfs/src

# Shared variables
ALLCSRC += $(FATFSSRC)
ALLINC  += $(FATFSINC)
