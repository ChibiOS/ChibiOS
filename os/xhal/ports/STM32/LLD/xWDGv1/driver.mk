ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_WDG TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/xWDGv1/hal_wdg_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/xWDGv1/hal_wdg_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/xWDGv1
