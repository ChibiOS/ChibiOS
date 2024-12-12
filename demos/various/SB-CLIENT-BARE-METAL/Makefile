##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === 128k code at 0x08060000, 16k data at 0x2001C000=================
	+@make --no-print-directory -f make/128k-08060000-16k-2001C000.make all
	@echo
	@echo === 128k code at 0x081E0000, 16k data at 0x20004000=================
	+@make --no-print-directory -f make/128k-081E0000-16k-20004000.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/128k-08060000-16k-2001C000.make clean
	@echo
	+@make --no-print-directory -f ./make/128k-081E0000-16k-20004000.make clean
	@echo

#
##############################################################################
