##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32F407-Olimex ==================================
	+@make --no-print-directory -f Makefile-stm32f407_olimex all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F746-Discovery ===============================
	+@make --no-print-directory -f Makefile-stm32f746_discovery all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F107-GoldBull ================================
	+@make --no-print-directory -f Makefile-stm32f107_goldbull all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f Makefile-stm32f407_olimex clean
	@echo
	+@make --no-print-directory -f Makefile-stm32f746_discovery clean
	@echo
	+@make --no-print-directory -f Makefile-stm32f107_goldbull clean
	@echo
#
##############################################################################
