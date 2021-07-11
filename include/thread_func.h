#ifndef THREAD_FUNC_H_
#define THREAD_FUNC_H_


#define WAIT_TIMEOUT 10
#define READ_THREAD_SLEEP 1200000
#define WRITE_THREAD_SLEEP 1000000
#define MIN_QUEUE_VALUE 1
#define MAX_QUEUE_VALUE 100

/**
 * This thread notifies the other threads about the status of the Queue (if full or empty)
 **/
void notifyStatus(Queue<int> *queue);

/**
 * This thread reads elements from the Queue.
 * If the quue is empty it waits for a new element to be added or times out
 **/
void reading_thread(Queue<int> *queue);

/**
 * This thread writes elements to the Queue.
 * If the quue is full it waits for a new element to be read/deleted or times out
 **/
void writing_thread(Queue<int> *queue);


#endif /* THREAD_FUNC_H_ */
