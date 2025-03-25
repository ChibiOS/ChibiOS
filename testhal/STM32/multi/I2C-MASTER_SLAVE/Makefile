##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32F407-Discovery ===============================
	+@make --no-print-directory -f ./make/stm32f407_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32G474RE-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32H750XB-Discovery =============================
	+@make --no-print-directory -f ./make/stm32h750xb_discovery.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32f407_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32h750xb_discovery.make clean
	@echo

#
##############################################################################
