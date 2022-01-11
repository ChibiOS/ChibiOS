ENTRY(_start)

PHDRS
{
  text PT_LOAD ;
  data PT_LOAD ;
}

SECTIONS
{
	.text 0x00000000 :
	{
		*(.text)
		*(.text.*)
	} : text

	.rodata :
	{
		*(.rodata)
		*(.rodata1)
		*(.rodata.*)
	}

	.data :
	{
		*(.data)
		*(.data1)
		*(.data.*)
	} : data

	.bss :
	{
		*(.bss)
		*(.bss.*)
		*(.sbss)
		*(.sbss.*)
		*(COMMON)
	}
}
