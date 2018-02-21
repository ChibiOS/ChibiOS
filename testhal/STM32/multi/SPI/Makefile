##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32F091-Nucleo64 ================================
	@make --no-print-directory -f Makefile-stm32f091_nucleo64 all
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
	@echo === Building for STM32H743-Nucleo144 ===============================
	@make --no-print-directory -f Makefile-stm32h743_nucleo144 all
	@echo ====================================================================
	@echo

clean:
	@echo
	-@make --no-print-directory -f Makefile-stm32f091_nucleo64 clean
	@echo
	-@make --no-print-directory -f Makefile-stm32f303_discovery clean
	@echo
	-@make --no-print-directory -f Makefile-stm32f407_discovery clean
	@echo
	-@make --no-print-directory -f Makefile-stm32h743_nucleo144 clean
	@echo

#
##############################################################################
