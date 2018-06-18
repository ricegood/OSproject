/********************************************************
 * Filename: core/sync.c
 *
 * Author: wsyoo, RTOSLab. SNU.
 *
 * Description: semaphore, condition variable management.
 ********************************************************/
#include <core/eos.h>

void eos_init_semaphore(eos_semaphore_t *sem, int32u_t initial_count, int8u_t queue_type) {
	/* initialization */
	sem->count = initial_count; // set initial count
	sem->wait_queue = NULL;
	sem->queue_type = queue_type; // set queue type
}

int32u_t eos_acquire_semaphore(eos_semaphore_t *sem, int32s_t timeout) {
	int32u_t saved_flags = eos_disable_interrupt(); // disable interrupt
	eos_tcb_t *current_task = eos_get_current_task(); // get current task

	while (1) {
		// semaphore acquire success
		printf("start acquire semaphore while loop\r\n");
		if (sem->count > 0) {
			sem->count--; // acquire
			eos_restore_interrupt(saved_flags); // enable interrupt
			printf("acquire semaphore : success\r\n");
			return 1; // return success
		}

		// can not acquire semaphore
		else {
			printf("acquire semaphore : start waiting\r\n");
			switch (timeout) {
				case -1: // acquire fail
					eos_restore_interrupt(saved_flags); // enable interrupt
					printf("acquire semaphore : timeout == -1, fail\r\n");
					return 0; // return fail
					break;

				default:  // wait until other task release it & time out end
					printf("racquire semaphore : timeout >= 0, state transition\r\n");
					current_task->state = 3; // WAITING, change current state
					printf("acquire semaphore : add to waiting queue\r\n");
					/* ====== segmentation fault here ====== */
					if (sem->queue_type == 0) // FIFO
						_os_add_node_tail(&(sem->wait_queue), &(current_task->node)); // add to wait queue
					else if(sem->queue_type == 1) // priority_based
						_os_add_node_priority(&(sem->wait_queue), &(current_task->node)); // add to wait queue
					printf("acquire semaphore : restore interrupt\r\n");
					eos_restore_interrupt(saved_flags);
					printf("acquire semaphore : eos_schedule()\r\n");
					eos_schedule(); // sleep this task
					printf("acquire semaphore : scheduling end, restart!\r\n");
					if(timeout > 0) {
						printf("acquire semaphore : timeout > 0\r\n");
						/*
          	if(timer->tick >= wait_time) {
	            eos_restore_interrupt(saved_flags);
              return 0;
            }
						*/
          }
					printf("acquire semaphore : acquire end\r\n");
					break;
			}
		}
	}
}

void eos_release_semaphore(eos_semaphore_t *sem) {
	printf("release semaphore\r\n");
	int32u_t saved_flags = eos_disable_interrupt(); // disable interrupt
	sem->count++;	// increase count
	if (sem->wait_queue != NULL) {
		_os_wakeup_single(&(sem->wait_queue), sem->queue_type);
	}
	eos_restore_interrupt(saved_flags);
}

void eos_init_condition(eos_condition_t *cond, int32u_t queue_type) {
	/* initialization */
	cond->wait_queue = NULL;
	cond->queue_type = queue_type;
}

void eos_wait_condition(eos_condition_t *cond, eos_semaphore_t *mutex) {
	/* release acquired semaphore */
	eos_release_semaphore(mutex);
	/* wait on condition's wait_queue */
	_os_wait(&cond->wait_queue);
	/* acquire semaphore before return */
	eos_acquire_semaphore(mutex, 0);
}

void eos_notify_condition(eos_condition_t *cond) {
	/* select a task that is waiting on this wait_queue */
	_os_wakeup_single(&cond->wait_queue, cond->queue_type);
}
