.global inb
.global outb

inb:
	mov 4(%esp), %dx /* port number is taken as an argument */
	in %dx, %al
	ret

outb:
	mov 8(%esp), %al
	mov 4(%esp), %dx
	out %al, %dx
	ret
