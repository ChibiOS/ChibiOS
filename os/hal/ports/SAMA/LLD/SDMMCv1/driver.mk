PLATFORMSRC +=	$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/sama_sdmmc_lld.c	\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ch_sdmmc_device.c	\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ch_sdmmc_sdio.c	\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ch_sdmmc_cmds.c	\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ch_sdmmc_mmc.c		\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ch_sdmmc_sd.c		\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ch_sdmmc.c			\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/osassert.c			\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/osoutput.c			\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/osclock.c			\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/osbdev.c			\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/osmutex.c			\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ostask.c			\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ch_sdmmc_reledge.c		\
				$(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1/ch_sdmmc_ff.c		
					
PLATFORMINC +=  $(CHIBIOS)/os/hal/ports/SAMA/LLD/SDMMCv1

