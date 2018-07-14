.global load_pdt

load_pdt:
	mov 4(%esp), %eax		/* takes in a page directory */
	mov %eax, %cr3			/* loads the pdt */
	
	mov %cr0, %eax			/* set PG and PE bits of %CR0 */
	or	$0x80000001, %eax
	mov %eax, %cr0
	
	mov %cr4, %eax			/* enable PSE (4 MiB pages) */
	or	$0x00000010, %eax
	mov %eax, %cr4

	ret
