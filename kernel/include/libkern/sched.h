#ifndef LIBKERN_SCHED_H
#define LIBKERN_SCHED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include <libkern/list.h>

struct proc {
	int pid;
	int prio;
	uint32_t esp;
	uint32_t ebp;
	uint32_t eip;
	struct list_head head;
};

#endif /* LIBKERN_SCHED_H */
