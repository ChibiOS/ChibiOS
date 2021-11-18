##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32F051-Discovery ===============================
	+@make --no-print-directory -f make/stm32f051_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F303_Discovery ==============================
	+@make --no-print-directory -f ./make/stm32f303_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F407-Discovery ===============================
	+@make --no-print-directory -f make/stm32f407_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32WB55RG_Nucleo68 =============================
	+@make --no-print-directory -f ./make/stm32wb55rg_nucleo68.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32f051_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32f303_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32f407_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32wb55rg_nucleo68.make clean
	@echo

#
##############################################################################
