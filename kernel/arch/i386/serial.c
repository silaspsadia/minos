#include <stdio.h>
#include <arch/i386/io.h>
#include <arch/i386/serial.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8 
#define COM4 0x2E8

void init_serial(void)
{
	outb(COM1 + 1, 0x00);    // Disable all interrupts
	outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(COM1 + 1, 0x00);    //                  (hi byte)
	outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty(void)
{
	return inb(COM1 + 5) & 0x20;
}

void serial_putchar(char a)
{
	while (is_transmit_empty() == 0)
		;
 
	outb(COM1, a);
}

void serial_write(const char *data, size_t size)
{
	int i = 0;
	while (i < size) {
		while (is_transmit_empty() == 0)
			;
		outb(COM1, data[i]);
		i++;
	}
}

void serial_writestring(const char *data)
{
	serial_write(data, strlen(data));
}
