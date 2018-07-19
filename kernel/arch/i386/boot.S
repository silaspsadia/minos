# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# This is the virtual base address of kernel space. It must be used to convert virtual
# addresses into physical addresses until paging is enabled. Note that this is not
# the virtual address where the kernel image itself is loaded -- just the amount that must
# be subtracted from a virtual address to get a physical address.
.set KERNEL_VIRTUAL_BASE, 0xC0000000                  # 3GB
.set KERNEL_PAGE_NUMBER, (KERNEL_VIRTUAL_BASE >> 22)  # Page directory index of kernel's 4MB PTE.

# Declares the boot Paging directory to load a virtual higher half kernel
.section .data
.align 0x1000
.global _boot_page_directory
_boot_page_directory:
    .long 0x00000083
    .fill (KERNEL_PAGE_NUMBER - 1), 4, 0x00000000
    .long 0x00000083
    .fill (1024 - KERNEL_PAGE_NUMBER - 1), 4, 0x00000000

.section .text
.global _loader
_loader:
  # Load Page Directory Base Register. Until paging is set up, the code must
  # be position-independent and use physical addresses, not virtual ones
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

  # Start fetching instructions in kernel space.
  # Since eip at this point holds the physical address of this command
  # (approximately 0x00100000) we need to do a long jump to the correct
  # virtual address of _start which is approximately 0xC0100000.
  movl $_start, %edx
  jmp *%edx

# The Higher-Half Kernel entry point.
.global _start
.type _start, @function
_start:
  # Unmap the identity-mapped first 4MB of physical address space.
  # movl $0, (_boot_page_directory) #uncommenting this crashes all, figure why
  invlpg (0)

  # Enter protection mode
  mov %cr0, %eax
  or $1, %al
  mov %eax, %cr0

  # Push multiboot info
  movl $stack_top, %esp
  push %ebx

  # Initialize the core kernel before running the global constructors.
  call kernel_init

  # Call the global constructors.
  call _init

  # Transfer control to the main kernel.
  call kernel_main

  # Hang if kernel_main unexpectedly returns.
  cli
.Lhang:
  hlt
  jmp .Lhang
.size _start, . - _start
