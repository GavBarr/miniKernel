ENTRY(_start)

SECTIONS {
	. = 0x100000;
	
	kernel_start = .;

	.text : ALIGN(4096)
	{
		*(.header)
		*(.text)	
	}

	.rodata : ALIGN(4096)
        {
                *(.rodata)
        }

        .data : ALIGN(4096)
        {
                *(.data)
        }


	.bss : ALIGN(4096)
	{
		*(.bss)
	}

	kernel_end = .;


}
