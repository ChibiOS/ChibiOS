ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_SDRAM TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS}/os/hal/ports/STM32/LLD/FSMCv1/hal_sdram_lld.c
endif
ifneq ($(findstring HAL_USE_SRAM TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS}/os/hal/ports/STM32/LLD/FSMCv1/hal_sram_lld.c
endif
ifneq ($(findstring HAL_USE_NAND TRUE,$(HALCONF)),)
PLATFORMSRC += ${CHIBIOS}/os/hal/ports/STM32/LLD/FSMCv1/hal_nand_lld.c
endif
else
PLATFORMSRC += ${CHIBIOS}/os/hal/ports/STM32/LLD/FSMCv1/hal_sram_lld.c  \
               ${CHIBIOS}/os/hal/ports/STM32/LLD/FSMCv1/hal_sdram_lld.c \
               ${CHIBIOS}/os/hal/ports/STM32/LLD/FSMCv1/hal_nand_lld.c
endif

PLATFORMINC += ${CHIBIOS}/os/hal/ports/STM32/LLD/FSMCv1
