ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_CAN TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/FDCANv1/hal_can_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/FDCANv1/hal_can_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/FDCANv1
