/********************************************************
 * Filename: core/task.c
 * 
 * Author: parkjy, RTOSLab. SNU.
 * 
 * Description: task management.
 ********************************************************/
#include <core/eos.h>

#define READY		1
#define RUNNING		2
#define WAITING		3

/*
 * Queue (list) of tasks that are ready to run.
 */
static _os_node_t *_os_ready_queue[LOWEST_PRIORITY + 1];

/*
 * Pointer to TCB of running task
 */
static eos_tcb_t *_os_current_task;

int32u_t eos_create_task(eos_tcb_t *task, addr_t sblock_start, size_t sblock_size, void (*entry)(void *arg), void *arg, int32u_t priority) {
	printf("===Start create task===\n");

	// Set TCB
	printf("Set TCB\n");
	task->priority = priority; // set tcb priority
	task->stkPtr = _os_create_context(sblock_start, sblock_size, entry, arg);	// set tcb stack pointer
	task->state = READY; // set tcb state

	// Create node
	printf("Create node\n");
	_os_node_t node;
	node.ptr_data = task;
	node.priority = priority;

	// Add node to ready queue
	printf("Add node to ready queue\n");
	_os_add_node_priority(&_os_ready_queue[priority], &node);

	printf("task stack pointer : %p\n", task->stkPtr);
	PRINT("task: 0x%x, priority: %d\n", (int32u_t)task, priority);
	int32u_t* sp = task->stkPtr;
	/*
	printf("print edi = %p\n", *(sp++));
	printf("print esi = %p\n", *(sp++));
	printf("print ebp = %p\n", *(sp++));
	printf("print esp = %p\n", *(sp++));
	printf("print ebx = %p\n", *(sp++));
	printf("print edx = %p\n", *(sp++));
	printf("print ecx = %p\n", *(sp++));
	printf("print eax = %p\n", *(sp++));
	printf("print eflags = %p\n", *(sp++));
	printf("print eip = %p\n", *(sp++));
	printf("print entry return address = %p\n", *(sp++));
	printf("print arg = %p\n", *(sp));
	printf("***print sp = %p\n", sp);
	*/

	printf("===End create task===\n\n");
	return 0;
}

int32u_t eos_destroy_task(eos_tcb_t *task) {
}

void eos_schedule() {
	printf("=====Start Scheduling=====\n");
	/* check current task */
	if (_os_current_task == NULL){
		printf("current task null!\n");
		printf("ready queue [0] : %p\n", _os_ready_queue[0]);
		int i = 0;
		while (_os_ready_queue[i] != NULL) {
			printf("ready queue [%d] : %p\n", i, _os_ready_queue[i]->ptr_data);
			i++;
		}
//		_os_restore_context(_os_ready_queue[0]->ptr_data);
	}
	else {
		printf("have task!\n");
		printf("current task tcb : %p\n", _os_current_task);
		printf("current task stack pointer : %p\n", _os_current_task->stkPtr);
		int32u_t stkPtr = _os_save_context();
		printf("stkPtr = %d\n", stkPtr);
		if (stkPtr == NULL) {
			// function termination
			printf("save context return null! \n");
			return;
		} else {
			printf("save context return stkPtr! \n");
			//_os_current_task->stkPtr = stkPtr; // save stkPtr to tcb
			//_os_restore_context(_os_ready_queue[0]->ptr_data); // restore context
		}
		
	}
}

eos_tcb_t *eos_get_current_task() {
	return _os_current_task;
}

void eos_change_priority(eos_tcb_t *task, int32u_t priority) {
}

int32u_t eos_get_priority(eos_tcb_t *task) {
}

void eos_set_period(eos_tcb_t *task, int32u_t period){
}

int32u_t eos_get_period(eos_tcb_t *task) {
}

int32u_t eos_suspend_task(eos_tcb_t *task) {
}

int32u_t eos_resume_task(eos_tcb_t *task) {
}

void eos_sleep(int32u_t tick) {
}

void _os_init_task() {
	PRINT("initializing task module.\n");

	/* init current_task */
	_os_current_task = NULL;

	/* init multi-level ready_queue */
	int32u_t i;
	for (i = 0; i < LOWEST_PRIORITY; i++) {
		_os_ready_queue[i] = NULL;
	}
}

void _os_wait(_os_node_t **wait_queue) {
}

void _os_wakeup_single(_os_node_t **wait_queue, int32u_t queue_type) {
}

void _os_wakeup_all(_os_node_t **wait_queue, int32u_t queue_type) {
}

void _os_wakeup_sleeping_task(void *arg) {
}
