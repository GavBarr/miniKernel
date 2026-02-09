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
} __attribute__((packed)) cpu_context;

struct task{
	int pid;
	task_state state;
	cpu_context *context;
	uint32_t esp; //stack pointer
	uint32_t *page_directory;
	uint32_t kernel_stack;
	
	int priority;
	uint32_t time_slice;
	
	struct task *parent;
	struct task *next;
	struct task *prev;
	
	
};

void task_init(void);
struct task *task_create(void (*entry_point)(void));
void schedule(void);

#endif
