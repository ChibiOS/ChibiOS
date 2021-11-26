##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32G474RE-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32L4R5ZI-Nucleo144 =============================
	+@make --no-print-directory -f ./make/stm32l4r9ai_discovery.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32l4r9ai_discovery.make clean
	@echo

#
##############################################################################
