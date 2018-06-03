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
	PRINT("task: 0x%x, priority: %d\n", (int32u_t)task, priority);

	// Set TCB
	task->priority = priority; // set tcb priority
	task->stkPtr = _os_create_context(sblock_start, sblock_size, entry, arg); // set tcb stack pointer
	task->state = READY; // set tcb state
	task->node.ptr_data = task; // set node pointer to tcb
	task->node.priority = priority; // set node priority
	task->period = 0;	// task period 0 (default: don't have period)
	task->nextPeriodStartTime = eos_get_system_timer()->tick; // default: don't have next Period Start Time
	printf("nextPeriodStartTime : %d", task->nextPeriodStartTime);
	// Add node to ready queue
	// printf("Add node to ready queue : %p\n", &(task->node));
	_os_add_node_priority(&_os_ready_queue[priority], &(task->node));

	// Add priority to map_table
	_os_set_ready(priority);

	return 0;

}

int32u_t eos_destroy_task(eos_tcb_t *task) {
}

void eos_schedule() {
	// get highest priority
	int32u_t highestPriority = _os_get_highest_priority();

	// check current task
	if (_os_current_task != NULL) {
		// current task != NULL
		int32u_t* stkPtr = (int32u_t *)_os_save_context();

		// current task is preempted
		if (_os_current_task->state == RUNNING) {
			_os_current_task->state = READY;
			_os_set_ready(_os_current_task->priority);
		}

		if (stkPtr == NULL) {
			// function termination
			return;
		} else {
			_os_current_task->stkPtr = stkPtr; // save stkPtr to tcb
			// add current task to ready queue
			_os_add_node_priority(&_os_ready_queue[_os_current_task->priority], &(_os_current_task->node));
		}
	}

	// set current task from ready queue
	_os_current_task = (eos_tcb_t*)(_os_ready_queue[highestPriority]->ptr_data);
	_os_current_task->state = RUNNING; // set tcb state

	// remove node from ready queue and restore
	_os_remove_node(&_os_ready_queue[_os_current_task->priority], &(_os_current_task->node));
	_os_restore_context(_os_current_task->stkPtr);

	// if ready queue becomes empty
  if(_os_ready_queue[highestPriority] == NULL)
    _os_unset_ready(highestPriority);
}

eos_tcb_t *eos_get_current_task() {
	return _os_current_task;
}

void eos_change_priority(eos_tcb_t *task, int32u_t priority) {
}

int32u_t eos_get_priority(eos_tcb_t *task) {
}

void eos_set_period(eos_tcb_t *task, int32u_t period){
	task->period = period;
}

int32u_t eos_get_period(eos_tcb_t *task) {
}

int32u_t eos_suspend_task(eos_tcb_t *task) {
}

int32u_t eos_resume_task(eos_tcb_t *task) {
}

void eos_sleep(int32u_t tick) {
	// if current task is period task
	if(_os_current_task->period != 0) {
		// save next period start time (current tick + period)
		_os_current_task->nextPeriodStartTime += _os_current_task->period;
		_os_current_task->state = WAITING; // set tcb state
		// Q. 여기 *,& 이런거 모르겠음 특히 콜백함수
		eos_alarm_t* newAlarm;
		eos_set_alarm(eos_get_system_timer(), newAlarm, _os_current_task->nextPeriodStartTime, _os_wakeup_sleeping_task, _os_current_task);
		eos_schedule(); // context switching
	}
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
	eos_tcb_t* task = (eos_tcb_t*)arg;
	task->state = READY; // state transition
	_os_set_ready(task->priority);
	_os_add_node_priority(&_os_ready_queue[task->priority], &(task->node)); // add to ready queue
}
