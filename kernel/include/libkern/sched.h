#ifndef LIBKERN_SCHED_H
#define LIBKERN_SCHED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include <libkern/list.h>

struct regs {
	uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t ss;
    uint32_t esp;
    uint32_t eflags;
    uint32_t cs;
    uint32_t eip;
};

struct proc {
	int pid;
	int prio;
	struct regs *reg_state
	struct list_head *head;
};

#endif /* LIBKERN_SCHED_H */
