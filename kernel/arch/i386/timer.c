#include <stdio.h>
#include <arch/i386/idt.h>
#include <arch/i386/interrupts.h>
#include <arch/i386/io.h>
#include <arch/i386/timer.h>
#include <libkern/time.h>

#define TICKS_PER_SECOND 100

void timer_phase(int hz)
{
	int divisor = 1193180 / hz;
	outb(0x43, 0x36); 
	outb(0x40, divisor & 0xFF);
	outb(0x40, divisor >> 8);
}

void timer_handler(struct regs *r)
{
	jiffies_64++;
	jiffies = jiffies_64;
#ifdef TIME_DISPLAY
	if (jiffies % TICKS_PER_SECOND == 0)
		printf("Time since install: %i s\r", jiffies / TICKS_PER_SECOND);
#endif
}

void timer_init(void)
{
	
	register_interrupt_handler(TIMER_IDT_INDEX, timer_handler);
	timer_phase(TICKS_PER_SECOND);
	jiffies = jiffies_64;
	printf("[Init ] Timer installed successfully\n");
}
