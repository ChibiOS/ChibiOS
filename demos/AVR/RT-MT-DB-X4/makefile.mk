
MSG_PROG  = -------- write the program to the device flash  --------
MSG_ERASE = -------- erase the device flash memory --------

DFU_PROGRAMMER = dfu-programmer
DFU_WRITE_FLASH = flash --force
DFU_ERASE_FLASH = erase
DFU_RESET       = reset

prog :
	@echo
	@echo $(MSG_PROG)
	$(DFU_PROGRAMMER) $(MCU) $(DFU_WRITE_FLASH) $(TARGET).hex
	$(DFU_PROGRAMMER) $(MCU) $(DFU_RESET)
	@echo $(MSG_END)

erase :
	@echo
	@echo $(MSG_ERASE)
	$(DFU_PROGRAMMER) $(MCU) $(DFU_ERASE_FLASH)
	@echo $(MSG_END)

