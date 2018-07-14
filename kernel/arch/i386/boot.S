# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Virtual memory base addresses
.set KERNEL_VIRTUAL_BASE, 0xC0000000
.set KERNEL_PAGE_NUMBER, (KERNEL_VIRTUAL_BASE >> 22);

.section .data
.align 0x1000
.global _boot_page_directory
_boot_page_directory:
	.long 0x00000083
	.fill (KERNEL_PAGE_NUMBER - 1), 4, 0x00000000
	.long 0x00000083
	.fill (1024 - KERNEL_PAGE_NUMBER - 1), 4, 0x00000000

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _loader
.type _loader, @function
_loader:
	mov $(_boot_page_directory - KERNEL_VIRTUAL_BASE), %ecx
	mov %ecx, %cr3

	# Set PSE bit in CR4 to enable 4MB pages.
	mov %cr4, %ecx
	or $0x00000010, %ecx
	mov %ecx, %cr4

	# Set PG bit in CR0 to enable paging.
	mov %cr0, %ecx
	or $0x80000000, %ecx
	mov %ecx, %cr0
	
	movl $_start, %edx
  	jmp *%edx
	
.global _start
.type _start, @function
_start:
	invlpg (0)
	movl $stack_top, %esp

	# Protection mode
	mov %cr0, %eax
	or $1, %al
	mov %eax, %cr0

	# Push multiboot info
	movl $stack_top, %esp
	push %ebx

	# Call kernel setup before global constructors
	call kernel_init

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size _start, . - _start
