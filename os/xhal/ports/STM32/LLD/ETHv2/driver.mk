ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_ETH TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/ETHv2/hal_eth_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/ETHv2/hal_eth_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/ETHv2
