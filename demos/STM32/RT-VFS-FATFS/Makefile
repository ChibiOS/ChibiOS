##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32F407ZG-Olimex ================================
	+@make --no-print-directory -f ./make/stm32f407zg_olimex.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F769ni-Discovery =============================
	+@make --no-print-directory -f ./make/stm32f769ni_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32G474RE-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32H735IG-Discovery =============================
	+@make --no-print-directory -f ./make/stm32h735ig_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32L4R5ZI-Discovery =============================
	+@make --no-print-directory -f ./make/stm32l4r9ai_discovery.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32f407zg_olimex.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32f769ni_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32h735ig_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32l4r9ai_discovery.make clean
	@echo

#
##############################################################################
