#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

void	serial_init(void);
int	is_transmit_empty(void);
void	serial_putchar(char);
void	serial_write(const char *, size_t);
void	serial_writestring(const char *);
