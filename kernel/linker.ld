ENTRY(_entry)

SECTIONS
{
	
	prekernel =.;
	
	. = 1M;

	begkernel = .;
	.text BLOCK(4K) : ALIGN(4K)
	{

		*(.multiboot)
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)	
		endkernel = .;
	}
	.junk BLOCK(4K) : ALIGN(4K)
	{
		*(.junk)

	}

}