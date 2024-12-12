##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === 128k code at 0x08040000, 16k data at 0x20018000=================
	+@make --no-print-directory -f make/128k-08040000-16k-20018000.make all
	@echo ====================================================================
	@echo
	@echo === 128k code at 0x081C0000, 16k data at 0x20000000=================
	+@make --no-print-directory -f make/128k-081C0000-16k-20000000.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/128k-08040000-16k-20018000.make clean
	@echo
	+@make --no-print-directory -f ./make/128k-081C0000-16k-20000000.make clean
	@echo

#
##############################################################################
