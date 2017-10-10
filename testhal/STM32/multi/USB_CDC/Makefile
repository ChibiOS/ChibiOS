##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32L476-Discovery ===============================
	@make --no-print-directory -f Makefile-stm32l476_discovery all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F303-Discovery ===============================
	@make --no-print-directory -f Makefile-stm32f303_discovery all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F407-Discovery ===============================
	@make --no-print-directory -f Makefile-stm32f407_discovery all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F746-Discovery ===============================
	@make --no-print-directory -f Makefile-stm32f746_discovery all
	@echo ====================================================================
	@echo

clean:
	@echo
	-@make --no-print-directory -f Makefile-stm32l476_discovery clean
	@echo
	-@make --no-print-directory -f Makefile-stm32f303_discovery clean
	@echo
	-@make --no-print-directory -f Makefile-stm32f407_discovery clean
	@echo
	-@make --no-print-directory -f Makefile-stm32f746_discovery clean
	@echo

#
##############################################################################
