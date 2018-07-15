/* The bootloader will look at this image and start execution at the symbol
   designated at the entry point. */
ENTRY(_loader)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 3 GiB + 1 MiB to mak e a higher-half kernel */
	. = 0xC0100000;
	kernel_vmem_start = .;
	kernel_pmem_start = . - 0xC0000000;	
	
	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text : AT(ADDR(.text)-0xC0000000)
	{
		*(.multiboot)
		*(.text)
		. = ALIGN(4K);
	}

	/* Read-only data. */
	.rodata : AT(ADDR(.rodata)-0xC0000000)
	{
		*(.rodata)
		. = ALIGN(4K);
	}

	/* Read-write data (initialized) */
	.data : AT(ADDR(.data)-0xC0000000)
	{
		*(.data)
		. = ALIGN(4K);
	}

	/* Read-write data (uninitialized) and stack */
	.bss : AT(ADDR(.bss)-0xC0000000)
	{
		*(COMMON)
		*(.bss)
		. = ALIGN(4K);
	}

	kernel_vmem_end = .;
	kernel_pmem_end = . - 0xC0000000;

	/* The compiler may produce other sections, put them in the proper place in
	   in this file, if you'd like to include them in the final kernel. */
}
