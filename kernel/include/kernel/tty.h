#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void kb_init(void);
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_backspace(void);
void terminal_scrolldown(void);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_centerwrite(const char* data);
void terminal_colorprint(const char* data, uint8_t fg);

#endif /* _KERNEL_TTY_H */
