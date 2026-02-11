#ifndef SCHED_H
#define SCHED_H
#include "task.h"

struct task_queue{

	struct task *head;
	struct task *tail;
	int count;

};

void scheduler_init(void);
void schedule(void);
#endif
