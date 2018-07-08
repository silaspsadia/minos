#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"
#include "idtc.h"
#include "io.h"
#include "kb.h"

static const uint16_t FB_COMMAND_PORT = 0x3D4;
static const uint16_t FB_DATA_PORT = 0x3D5;
static const uint8_t FB_HIGH_BYTE_COMMAND = 14;
static const uint8_t FB_LOW_BYTE_COMMAND = 15;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH_CENTER = 39;
static const size_t VGA_HEIGHT_CENTER = 12;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void set_cursor(unsigned short pos) {
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, (pos >> 8) & 0x00FF);
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}

void kb_init(void) {
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	outb(0x21 , 0xFD);
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	// Logical characters shouldn't be rendered, but processed instead
	if (c == '\n') {
		terminal_row++;
		terminal_column = 0;
		set_cursor(terminal_row * VGA_WIDTH + terminal_column); 
		return;
	}	
	unsigned char uc = c;
	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	set_cursor(terminal_row * VGA_WIDTH + terminal_column + 1);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		// When reaches end of frame, enable (basic) terminal scrolling
		if (++terminal_row == VGA_HEIGHT) {	
			for (size_t y = 1; y < VGA_HEIGHT; y++) {
				for (size_t x = 0; x < VGA_WIDTH; x++) {
					terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
				}
			}
			size_t end_row_index = (VGA_HEIGHT - 1) * VGA_WIDTH; 
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				terminal_buffer[end_row_index + x] = vga_entry(' ', terminal_color);
			}
			terminal_row = VGA_HEIGHT - 1;
			set_cursor(end_row_index);
		}
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_centerwrite(const char* data) {
	size_t len = strlen(data);
	size_t adjust = len / 2;
	if (adjust > VGA_WIDTH) 
		return;
	terminal_row++;
	terminal_column = VGA_WIDTH_CENTER - adjust; 
	terminal_writestring(data);
}

void keyboard_handler_main(void) {
	terminal_writestring("WOWWOWOWOWOWOWOWWOW\n");
	unsigned char status;
	char keycode;

	/* write EOI */
	outb(0x20, 0x20);

	status = inb(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = inb(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;
		terminal_putchar(keyboard_map[(unsigned char) keycode]);
	}
}
