ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_EXT TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/STM32/LLD/EXTIv1/hal_ext_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/STM32/LLD/EXTIv1/hal_ext_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/hal/ports/STM32/LLD/EXTIv1
