#include "../include/kernel/sched.h"
#include "../include/kernel/task.h"
#include "../include/kernel/config.h"
#include "../mem_alloc/heap.h"
#include <stddef.h>
#include <stdint.h>


/*struct task_queue{

        struct task *head;
        struct task *tail;
        int count;

};
*/


struct task_queue ready_queue;
struct task_queue blocked_queue;
struct task *current_task;
struct task *idle_task;
int next_pid;

static void task_init_stack(struct task *task, void (*entry_point)(void));
static void idle_task_function(void);

static void idle_task_function(void){
	while (1){
		__asm__ volatile("hlt");
	}
}

/* schedules the next task accordingly to the queue
*/
void schedule(void){
	if (ready_queue.count == 0) return;

	//if the state is TASK_READY then we need to schedule that as the next task
	//to run, otherwise go down the queue further		if (ready_queue.head == NULL) return;
	if (ready_queue.head->state == 0){
		current_task = ready_queue.head;
		ready_queue.head = current_task->next;
		return;	
	}
		

	return;
	
}



void scheduler_init(void){
	
	current_task = kmalloc(sizeof(struct task));
	idle_task = kmalloc(sizeof(struct task));

	ready_queue.head = NULL;
	ready_queue.tail = NULL;
	ready_queue.count = 0;
	
	blocked_queue.head = NULL;
        blocked_queue.tail = NULL;
        blocked_queue.count = 0;
	
	next_pid = 1;

	idle_task->pid = 0;
	idle_task->state = TASK_RUNNING;
	idle_task->priority = 0; //lowest
	idle_task->kernel_stack = (uint32_t)kmalloc(KERNEL_STACK_SIZE);
	
	idle_task->parent = NULL;
	idle_task->next = NULL;
	idle_task->prev = NULL;

	task_init_stack(idle_task, idle_task_function);

	current_task = idle_task;
	

			
}





static void task_init_stack(struct task *task, void (*entry_point)(void)){

	uint32_t stack_top = task->kernel_stack + KERNEL_STACK_SIZE;
	
	stack_top -= sizeof(cpu_context);


	cpu_context *context = (cpu_context *)stack_top;


	context->edi = 0;
	context->esi = 0;
	context->ebx = 0;
	context->ebp = 0;
	context->eip = (uint32_t)entry_point; //this will be at the top of the stack, and will be executed as soon as RET happens

	
	task->context = context;
	task->esp = (uint32_t)context;
	

}














