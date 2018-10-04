ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_QSPI TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/STM32/LLD/QUADSPIv1/hal_qspi_lld.c
endif
ifneq ($(findstring HAL_USE_WSPI TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/STM32/LLD/QUADSPIv1/hal_wspi_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/STM32/LLD/QUADSPIv1/hal_qspi_lld.c
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/STM32/LLD/QUADSPIv1/hal_wspi_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/hal/ports/STM32/LLD/QUADSPIv1
