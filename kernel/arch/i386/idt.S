.global idt_load
.global keyboard_handler

.extern keyboard_handler_main
  
  
keyboard_handler:
	call    keyboard_handler_main
	iret

idt_load:
	mov 4(%esp), %eax
	lidt (%eax)
	sti
	ret 
