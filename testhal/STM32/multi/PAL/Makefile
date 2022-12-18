##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32F051-Discovery ===============================
	+@make --no-print-directory -f make/stm32f051_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F103-Olimex ==================================
	+@make --no-print-directory -f make/stm32f103_olimex.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F303-Discovery ===============================
	+@make --no-print-directory -f make/stm32f303_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F373-Eval ====================================
	+@make --no-print-directory -f make/stm32f373_eval.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F407-Discovery ===============================
	+@make --no-print-directory -f make/stm32f407_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F429-Discovery ===============================
	+@make --no-print-directory -f make/stm32f429_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F746-Discovery ===============================
	+@make --no-print-directory -f make/stm32f746_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32L053-Discovery ===============================
	+@make --no-print-directory -f make/stm32l053_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32L152RB-Discovery ===============================
	+@make --no-print-directory -f make/stm32l152rb_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32L476-Discovery ===============================
	+@make --no-print-directory -f make/stm32l476_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32WL55JC_Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32WB55RG_Nucleo68 ==============================
	+@make --no-print-directory -f ./make/stm32wb55rg_nucleo68.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f make/stm32f051_discovery.make clean
	@echo
	+@make --no-print-directory -f make/stm32f103_olimex.make clean
	@echo
	+@make --no-print-directory -f make/stm32f303_discovery.make clean
	@echo
	+@make --no-print-directory -f make/stm32f373_eval.make clean
	@echo
	+@make --no-print-directory -f make/stm32f407_discovery.make clean
	@echo
	+@make --no-print-directory -f make/stm32f429_discovery.make clean
	@echo
	+@make --no-print-directory -f make/stm32f746_discovery.make clean
	@echo
	+@make --no-print-directory -f make/stm32l053_discovery.make clean
	@echo
	+@make --no-print-directory -f make/stm32l152rb_discovery.make clean
	@echo
	+@make --no-print-directory -f make/stm32l476_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32wb55rg_nucleo68.make clean
	@echo

#
##############################################################################
