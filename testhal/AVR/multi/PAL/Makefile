##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for Arduino-Mega ======================================
	@make --no-print-directory -f Makefile-arduino_mega all
	@echo ====================================================================
	@echo
	@echo === Building for Arduino-Mini ======================================
	@make --no-print-directory -f Makefile-arduino_mini all
	@echo ====================================================================
	@echo
	@echo === Building for Arduino-Nano ======================================
	@make --no-print-directory -f Makefile-arduino_nano all
	@echo ====================================================================
	@echo
	@echo === Building for Arduino-Uno =======================================
	@make --no-print-directory -f Makefile-arduino_uno all
	@echo ====================================================================
	@echo

clean:
	@echo
	-@make --no-print-directory -f Makefile-arduino_mega clean
	@echo
	-@make --no-print-directory -f Makefile-arduino_mini clean
	@echo
	-@make --no-print-directory -f Makefile-arduino_nano clean
	@echo
	-@make --no-print-directory -f Makefile-arduino_uno clean
	@echo

#
##############################################################################
