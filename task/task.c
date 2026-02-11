#include "../debug/debug.h"
#include "../include/kernel/sched.h"
#include "../include/kernel/task.h"
#include "../include/kernel/config.h"
#include "../mem_alloc/heap.h"
#include <stdint.h>
#include <stddef.h>


int latest_pid = 0;
static int get_next_pid();

void print_task(struct task *task){
	print_string("--task--\n\0");
	print_string("pid->\0");
	print_int(task->pid);
	print_string("\n\0");
	print_string("state->\0");
        print_int(task->state);
	print_string("\n\0");
	print_string("kernel_stack->\0");
        print_pointer((void *)task->kernel_stack);
	print_string("\n\0");
	print_string("priority->\0");
        print_int(task->priority);
	print_string("\n\0");
	print_string("parent->\0");
        print_pointer(task->parent);
        print_string("\n\0");
	print_string("next->\0");
        print_pointer(task->next);
        print_string("\n\0");
	print_string("prev->\0");
        print_pointer(task->prev);
        print_string("\n\0");
	print_string("esp->\0");
        print_pointer((uint32_t *)task->esp);
        print_string("\n\0");
}

struct task *task_create(void (*entry_point)(void)){
	int pid = get_next_pid();

	struct task *new_task = kmalloc(sizeof(struct task));
	new_task->pid = pid;
	new_task->state = TASK_READY;
	new_task->kernel_stack = (uint32_t)kmalloc(KERNEL_STACK_SIZE);
	new_task->priority = 0;
	new_task->parent = NULL;
	new_task->next = NULL;
	new_task->prev = NULL;

	uint32_t stack_top = new_task->kernel_stack + KERNEL_STACK_SIZE;
	stack_top -= sizeof(cpu_context);
	
	cpu_context *context = (cpu_context *)stack_top;
	
	context->edi = 0;
	context->esi = 0;
	context->ebx = 0;
	context->ebp = 0;
	context->eip = (uint32_t)entry_point;

	new_task->context = context;
	new_task->esp = (uint32_t)context;
	
	return new_task;
}

int task_destroy(struct task *task){
	kfree((void *)task->kernel_stack);
	kfree(task);

	return 0;
}
int task_set_state(task_state state);
task_state task_get_state(void);


static int get_next_pid(){
	latest_pid++;
	return latest_pid;
}

