/********************************************************
 * Filename: core/comm.c
 *
 * Author: jtlim, RTOSLab. SNU.
 *
 * Description: message queue management.
 ********************************************************/
#include <core/eos.h>

void eos_init_mqueue(eos_mqueue_t *mq, void *queue_start, int16u_t queue_size, int8u_t msg_size, int8u_t queue_type) {
  //printf("init mq\r\n");
  mq->queue_start = (int8u_t *)queue_start;
  mq->front = (int8u_t *)queue_start;
  mq->rear = (int8u_t *)queue_start;
  mq->queue_size = queue_size;
  mq->msg_size = msg_size;
  mq->queue_type = queue_type;
  eos_init_semaphore(&(mq->putsem), queue_size, queue_type);
  eos_init_semaphore(&(mq->getsem), 0, queue_type);
}

int8u_t eos_send_message(eos_mqueue_t *mq, void *message, int32s_t timeout) {
  //printf("send mq\r\n");
  if (eos_acquire_semaphore(&(mq->putsem), timeout) == 0) {
    return;
  }
  // success to get semaphore
  else {
    //printf("success to get semaphore(putsem)\r\n");
    memcpy(mq->rear, message, mq->msg_size); // copy the message on the rear
    (mq->rear) += mq->msg_size; // update rear
    if(mq->rear >= mq->queue_start + mq->queue_size)
      mq->rear = mq->queue_start;
    eos_release_semaphore(&(mq->getsem)); // release semaphore
  }
}

int8u_t eos_receive_message(eos_mqueue_t *mq, void *message, int32s_t timeout) {
  //printf("receive mq\r\n");
  if (eos_acquire_semaphore(&(mq->getsem), timeout) == 0) {
    //printf("fail to get semaphore (getsem)\r\n");
    // fail to get semaphore
    return;
  }

  // success to get semaphore
  else {
    //printf("success to get semaphore (getsem)\r\n");
    memcpy(message, mq->front, mq->msg_size); // copy the message on the front
    (mq->front) += mq->msg_size; // update front
    if(mq->front >= mq->queue_start + mq->queue_size)
      mq->front = mq->queue_start;
    eos_release_semaphore(&(mq->putsem)); // release semaphore
  }
}
