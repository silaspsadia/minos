.global gdt_load_and_set

.section .text

gdt_load_and_set:
	mov 4(%esp), %eax
	lgdt (%eax)
	
	/* Load segment via far jmp */
	jmp $0x08, $.reload_segments

.reload_segments:
    /* We only use one segment for data */
    mov     $0x10, %ax
    mov     %ax, %ds
    mov     %ax, %ss
    mov     %ax, %es
    mov     %ax, %gs
    mov     %ax, %fs
    ret
