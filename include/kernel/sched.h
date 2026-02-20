#ifndef SCHED_H
#define SCHED_H
#include "task.h"

struct task_queue{

	struct task *head;
	struct task *tail;
	int count;

};

struct task *return_ready_queue_tail(void);
void scheduler_init(void);
void schedule(void);
void switch_task(struct task *old, struct task *new);
void enqueue_task(struct task *task);
void dequeue_task(void);
void task_wrapper(void);
void exit_task(void);
#endif
