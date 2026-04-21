ifeq ($(USE_SMART_BUILD),yes)
  ifneq ($(findstring HAL_USE_I2C TRUE,$(HALCONF)),)
    PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/I2Cv4/hal_i2c_lld.c
  endif
else
  PLATFORMSRC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/I2Cv4/hal_i2c_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/I2C
PLATFORMINC += $(CHIBIOS)/os/xhal/ports/STM32/LLD/I2Cv4
