#ifndef I386_TIMER_H
#define I386_TIMER_H
#include <stddef.h>
#include <stdint.h>

void timer_phase(int);
void timer_handler(struct regs *);
void timer_install(void);

#endif /* I386_TIMER_H */
