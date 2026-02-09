#ifndef SCHED_H
#define SCHED_H
#include "task.h"

struct task_queue{

	struct task *head;
	struct task *tail;
	int count;

};

void scheduler_init(void);
//struct task_struct *task_create(void(*entry_point)(void), int priority);

//void task_destroy(struct task_struct *task);

//void task_set_state(struct task_struct *task, task_state new_state);

//struct task_struct *get_current_task(void);

//void set_current_task(struct task_struct *task);

//struct task_struct *pick_next_task(void);

//void schedule(void);
#endif
