#include "../debug/debug.h"
#include "../include/kernel/sched.h"
#include "../include/kernel/task.h"
#include "../include/kernel/config.h"
#include "../mem_alloc/heap.h"
#include <stdint.h>
#include <stddef.h>
#define MAX_NUMBER_OF_TASKS 12

struct running_tasks *task_list;
int latest_pid = 0;
static int get_next_pid();



void tasks_array_init(void){
	task_list = kmalloc(sizeof(struct running_tasks) * MAX_NUMBER_OF_TASKS);
	for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++){
		task_list[i].used = 0;
		task_list[i].task = NULL;
	}
}

int find_task(int pid){
	
	for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++){
		if (task_list[i].used != 1) continue;
		if (task_list[i].task->pid == pid) return i;
	}

	return -1;
}



int add_task_to_array(struct task *task){
	for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++){
		if (task_list[i].used == 1) continue;
		task_list[i].used = 1;
		task_list[i].task = task;
		return 0;
	}
	return -1;
}

int remove_task_from_array(struct task *task){
	for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++){
		if (task_list[i].used == 0) continue;
		if (task_list[i].task != task) continue;
		task_list[i].used = 0;
		task_list[i].task = NULL;
		return 0;
	}
	return -1;
}

struct task *get_task(int pid){
	
	for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++){
		if (task_list[i].task->pid == pid) return task_list[i].task;
	}
	return NULL;
}

int return_task_list(int *list){
		
	int j = 0;
	for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++){
		if (task_list[i].used == 0) continue;
		list[j] = task_list[i].task->pid;
		j++;
	}
	
	return j;

}

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
//	print_string("esp->\0");
//        print_pointer((uint32_t *)task->esp);
//        print_string("\n\0");
}

struct task *task_create(void (*entry_point)(void), uint32_t priority){
	int pid = get_next_pid();
	
	struct task *new_task = kmalloc(sizeof(struct task));
	new_task->pid = pid;
	new_task->state = TASK_READY;
	new_task->kernel_stack = (uint32_t)kmalloc(KERNEL_STACK_SIZE);
	new_task->priority = priority;
	new_task->parent = NULL;
	new_task->next = NULL;
	new_task->prev = NULL;
	new_task->entry_point = entry_point;
	

	uint32_t stack_top = new_task->kernel_stack + KERNEL_STACK_SIZE;
	stack_top -= sizeof(cpu_context);
	
	cpu_context *context = (cpu_context *)stack_top;
	
	context->edi = 0;
	context->esi = 0;
	context->ebx = 0;
	context->ebp = 0;
	context->eip = (uint32_t)task_wrapper;
	context->esp = stack_top + sizeof(cpu_context);


	new_task->context = context;
	
	enqueue_task(new_task); //need to put this task
	add_task_to_array(new_task);	
	return new_task;
}

int task_destroy(int pid){
	int array_index = find_task(pid);
	if (array_index == -1) return -1;

	task_list[array_index].task->state = TASK_ZOMBIE;
	task_list[array_index].task = NULL;
	task_list[array_index].used = 0;
	
	return 0;
}

//void task_block(struct task *task){
//	task->state = TASK_BLOCKED;

//	schedule();
//}

//void task_unblock(struct task *task){
//	task->state = TASK_READY;

	//schedule();
//}

int task_set_state(struct task *task ,task_state state){
	
	task->state = state;
	return 0;
}
task_state task_get_state(struct task *task){
	
	return task->state;
	
}


static int get_next_pid(){
	latest_pid++;
	return latest_pid;
}

