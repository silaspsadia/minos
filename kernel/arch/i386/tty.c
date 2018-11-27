#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include <arch/i386/vga.h>
#include <arch/i386/io.h>

static const uint16_t FB_COMMAND_PORT = 0x3D4;
static const uint16_t FB_DATA_PORT = 0x3D5;
static const uint8_t FB_HIGH_BYTE_COMMAND = 14;
static const uint8_t FB_LOW_BYTE_COMMAND = 15;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define  VGA_WIDTH_CENTER 39

static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint8_t terminal_fg;
static uint8_t terminal_bg;
static uint16_t* terminal_buffer;

uint16_t line_fill[VGA_HEIGHT];

void set_cursor(unsigned short x, unsigned short y)
{
	unsigned short pos = y * VGA_WIDTH + x;
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, (pos >> 8) & 0x00FF);
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;	
	terminal_fg = VGA_COLOR_LIGHT_GREY;
	terminal_bg = VGA_COLOR_BLACK;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	// Logical characters shouldn't be rendered, but processed instead
	line_fill[terminal_row] = terminal_column;
	switch (c) {
	case '\n':	
		if (terminal_row + 1 == VGA_HEIGHT) {
			terminal_scrolldown();
			return;
		}
		terminal_row++;
		terminal_column = 0;
		set_cursor(terminal_column, terminal_row); 
		break;
		
	case '\r':
		terminal_column = 0;
		break;

	default: ;
		unsigned char uc = c;
		terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
		set_cursor(terminal_column + 1, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			// When reaches end of frame, enable (basic) terminal scrolling
			if (++terminal_row == VGA_HEIGHT)	
				terminal_scrolldown();
		}
		break;
		
	}
	return;
}

void terminal_backspace(void)
{
	if (terminal_column == 0) {
		if (terminal_row == 0)
			return;
		terminal_row--;
		terminal_column = line_fill[terminal_row];
	} else {
		terminal_column--;
	}
	terminal_putentryat('\0', terminal_color, terminal_column, terminal_row);
	set_cursor(terminal_column, terminal_row);	
}

void terminal_scrolldown(void)
{
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++)
			terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
		line_fill[y-1] = line_fill[y];
	}
	terminal_column = 0;
	terminal_row = VGA_HEIGHT - 1;
	set_cursor(terminal_column, terminal_row);
	for (size_t x = 0; x < VGA_WIDTH; x++)
  		terminal_putentryat(' ', terminal_color, x, terminal_row);
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_centerwrite(const char* data)
{
	size_t len = strlen(data);
	size_t adjust = len / 2;
	if (adjust > VGA_WIDTH) 
		return;
	terminal_row++;
	terminal_column = VGA_WIDTH_CENTER - adjust; 
	terminal_writestring(data);
}

void terminal_colorprint(const char* data, uint8_t fg)
{
	uint8_t save = terminal_color;
	terminal_color = fg | terminal_bg << 4;
	terminal_writestring(data);
	terminal_color = save;
}
