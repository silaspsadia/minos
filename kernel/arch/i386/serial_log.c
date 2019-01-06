#include <stdio.h>
#include <stddef.h>
#include <arch/i386/serial.h>
#include <kernel/serial_log.h>

void serial_debug_print(const char *data)
{
	printf("%s", data);
	serial_writestring(data);
}
