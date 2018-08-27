#include <stdio.h>
#include <arch/i386/idt.h>
#include <arch/i386/interrupts.h>
#include <arch/i386/io.h>
#include <arch/i386/timer.h>

#define TICKS_PER_SECOND 100

unsigned int _timer_ticks;

void timer_phase(int hz) 
{
	int divisor = 1193180 / hz;
	outb(0x43, 0x36); 
	outb(0x40, divisor & 0xFF);
	outb(0x40, divisor >> 8);
}

void timer_handler(struct regs *r) 
{
	_timer_ticks++;
}

void timer_init(void)
{
	register_interrupt_handler(TIMER_IDT_INDEX, timer_handler);
	timer_phase(TICKS_PER_SECOND);
	printf("[Init ] Timer installed successfully\n");
}
