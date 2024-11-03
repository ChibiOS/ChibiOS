##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32G474RE-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32H563ZI-Nucleo144 ===============================
	+@make --no-print-directory -f ./make/stm32h563zi_nucleo144.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32U575ZI-Nucleo144 ===============================
	+@make --no-print-directory -f ./make/stm32u575zi_nucleo144.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32h563zi_nucleo144.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32u575zi_nucleo144.make clean
	@echo

#
##############################################################################
