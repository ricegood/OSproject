/********************************************************
 * Filename: core/sync.c
 *
 * Author: wsyoo, RTOSLab. SNU.
 *
 * Description: semaphore, condition variable management.
 ********************************************************/
#include <core/eos.h>

void eos_init_semaphore(eos_semaphore_t *sem, int32u_t initial_count, int8u_t queue_type) {
	printf("init semaphore\r\n");
	/* initialization */
	sem->count = initial_count; // set initial count
	sem->wait_queue = NULL;
	sem->queue_type = queue_type; // set queue type
}

int32u_t eos_acquire_semaphore(eos_semaphore_t *sem, int32s_t timeout) {
	int32u_t saved_flags = eos_disable_interrupt(); // disable interrupt
	eos_tcb_t *current_task = eos_get_current_task(); // get current task
	eos_counter_t *timer = eos_get_system_timer();
  int32u_t wait_time;
	printf("acquire1 semaphore\r\n");
	if(timeout == -1) {
        wait_time = timer -> tick + timeout;
        eos_set_alarm(timer, &(current_task -> alarm), wait_time, _os_wakeup_sleeping_task, current_task);
  }
	printf("acquire2 semaphore\r\n");
	while (1) {
		// semaphore acquire success
		if (sem->count > 0) {
			sem->count--; // acquire
			eos_restore_interrupt(saved_flags); // enable interrupt
			return 1; // return success
			printf("acquire3 semaphore\r\n");
		}

		// can not acquire semaphore
		else {
			switch (timeout) {
				case -1: // acquire fail
					eos_restore_interrupt(saved_flags); // enable interrupt
					printf("acquire4 semaphore\r\n");
					return 0; // return fail
					break;

				default:  // wait until other task release it & time out end
					current_task->state = 3; // WAITING, change current state
					if (sem->queue_type == 0) // FIFO
						_os_add_node_tail(&(sem->wait_queue), &(current_task->node)); // add to wait queue
					else if(sem->queue_type == 1) // priority_based
						_os_add_node_priority(&(sem->wait_queue), &(current_task->node)); // add to wait queue
						printf("acquire5 semaphore\r\n");
					eos_schedule(); // sleep this task
					printf("acquire6 semaphore\r\n");
					if(timeout > 0) {
						printf("acquire7 semaphore\r\n");
          	if(timer->tick >= wait_time) {
							printf("acquire8 semaphore\r\n");
	            eos_restore_interrupt(saved_flags);
							printf("acquire9 semaphore\r\n");
              return 0;
            }
          }

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
