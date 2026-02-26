#include "../debug/debug.h"
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
static int debug = 0;
extern void context_switch(cpu_context *old, cpu_context *new);

struct task_queue ready_queue;
struct task_queue blocked_queue;
struct task *current_task;
struct task *idle_task;
int next_pid;

static void task_init_stack(struct task *task, void (*entry_point)(void));
static void idle_task_function(void);

static void idle_task_function(void){
//	print_string("IDLE TASK FUNCTION\n\0");
	while (1){
		__asm__ volatile("hlt");
	}
}

struct task *return_ready_queue_tail(void){
	return ready_queue.tail;
}

void yield(void){
	//current_task->state = TASK_READY;
	schedule();
}

void switch_task(struct task *old, struct task *new){
	if (debug){
		print_string("*old*\n\0");
	//print_pointer(old->context);
	//print_pointer(new->context);
		print_string("edi->\0");
		print_int(old->context->edi);
		print_string("\n\0");
		print_string("esi->\0");
	        print_int(old->context->esi);
	        print_string("\n\0");
		print_string("ebx->\0");
	        print_int(old->context->ebx);
	        print_string("\n\0");
		print_string("ebp->\0");
	        print_int(old->context->ebp);
	        print_string("\n\0");
		print_string("eip->\0");
	        print_pointer((void *)old->context->eip);
	        print_string("\n\0");
		print_string("esp->\0");
	        print_pointer((void *)old->context->esp);
	        print_string("\n\0");

		print_string("*new*\n\0");
	        print_string("edi->\0");
	       	print_int(new->context->edi);
	        print_string("\n\0");
	        print_string("esi->\0");
	        print_int(new->context->esi);
	        print_string("\n\0");
	        print_string("ebx->\0");
	        print_int(new->context->ebx);
	        print_string("\n\0");
	        print_string("ebp->\0");
	        print_int(new->context->ebp);
	        print_string("\n\0");
	        print_string("eip->\0");
	        print_pointer((void *)new->context->eip);
	        print_string("\n\0");
	        print_string("esp->\0");
	        print_pointer((void *)new->context->esp);
	        print_string("\n\0");
	}

	context_switch(old->context, new->context);
}

void task_wrapper() {
	current_task->entry_point();
	exit_task();
}

void exit_task(){
	current_task->state = TASK_ZOMBIE;
	//dequeue_task();
	//kfree(current_task);
	schedule();
}

/**
* @brief dequeues the task in the head of the READY queue 
*
* @param void
* @return nothing
*/
void dequeue_task(void){
	
	ready_queue.head = ready_queue.head->next;
	ready_queue.count--;
}

/**
 * @brief enqueues task to the tail of the READY queue  
 *
 * @param  task  
 */
void enqueue_task(struct task *task){
	// if nothing is in the queue we need to set the head and tail to the only task, which is the incoming task
	if (ready_queue.count == 0){
		ready_queue.head = task;
		ready_queue.tail = task;
		task->next = task;
	}else{
	//if there is already an existing task in the queue, then we MUST make sure the pointers are correct
		ready_queue.tail->next = task;
		task->prev = ready_queue.tail;
		ready_queue.tail = task;
	}

	ready_queue.count++;
}

/* schedules the next task accordingly to the queue
*/
void schedule(void){
	if (ready_queue.count == 0){
		return;
	}

	if (current_task->time_slice <= current_task->priority){
		current_task->time_slice++;
		return;
	}else{
		current_task->time_slice=0;
	}

	struct task *new_task = ready_queue.head;

        dequeue_task();
	
	if (new_task->state != TASK_READY){
		return; //skip for now until ready
	}

	//to run, otherwise go down the queue further		if (ready_queue.head == NULL) return;
	struct task *old_task = current_task;
	if (old_task != idle_task && old_task->state != TASK_ZOMBIE){
		old_task->state = TASK_READY;
		//print_string("\nenqueue(old_task)->\0");
		//print_pointer(old_task);
		enqueue_task(old_task);
	}

	current_task = new_task;
        current_task->state = TASK_RUNNING;

	//print_string("switch_task(old_task->\0");
	//print_pointer(old_task);
	//print_string(" pid->\0");
	//print_int(old_task->pid);
	//print_string("\n\0");
	//print_string("switch_task(new_task->\0");
	//print_pointer(new_task);
	//print_string(" pid->\0");
	//print_int(new_task->pid);
	//print_string("\n\0");

	switch_task(old_task, new_task);
		

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
	idle_task->entry_point = idle_task_function;
	task_init_stack(idle_task, idle_task_function);

	current_task = idle_task;
	//enqueue_task(idle_task);

	tasks_array_init();	
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
	context->esp = stack_top;
	
	task->context = context;
	

}






static inline void outb(uint16_t port, uint8_t value){
        //porting I/O

        //assembly instructions ot put value into AL register and put port into DX register
        __asm__ volatile ("outb %0, %1"
                          :
                          : "a"(value),"Nd"(port)
                          );

}

static inline uint8_t inb(uint16_t port){
        uint8_t return_val;

        __asm__ volatile ("inb %1, %0"
                         : "=a"(return_val)
                         : "Nd"(port)
                         );

        return return_val;
}








