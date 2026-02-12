#ifndef TASK_H
#define TASK_H
#include <stdint.h>


typedef enum{
	TASK_READY,
	TASK_RUNNING,
	TASK_BLOCKED,
	TASK_ZOMBIE
} task_state;

typedef struct cpu_context{
	//register values needed to be saved during the switch
	uint32_t edi;
	uint32_t esi;
	uint32_t ebx;
	uint32_t ebp;
	uint32_t eip;
	uint32_t esp;
} __attribute__((packed)) cpu_context;

struct task{
	int pid;
	task_state state;
	cpu_context *context;
	//uint32_t esp; //stack pointer
	uint32_t *page_directory;
	uint32_t kernel_stack;
	void (*entry_point)(void);	
	int priority;
	uint32_t time_slice;
	
	struct task *parent;
	struct task *next;
	struct task *prev;
	
	
};

struct task *task_create(void (*entry_point)(void));
int task_destroy(struct task *task);
int task_set_state(struct task *task, task_state state);
task_state task_get_state(struct task *task);
void print_task(struct task *task);


#endif
