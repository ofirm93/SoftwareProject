#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>

/**
 * SP Bounded Priority Queue summary
 *
 * This implementation of Priority Queue done using cyclic array.
 * It keeps the array sorted from the starting index (which might won't be zero) till the ending index.
 * Moreover it uses lazy techniques in order to achieve better performance.
 *
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

typedef struct sp_bpq_element_t{
	int index;
	double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Creator function for BPQ (Bounded Priority Queue).
 * It creates a BPQ with upper boundry maxSize.
 * If maxSize if lower than 0 , or if there is not enough memory to create the queue, it returns NULL.
 *
 * @param maxSize - the maximum capacity of the new queue.
 *
 * @return SPBPQueue* for an empty BPQ or NULL as mentioned above.
 */
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * This function gets two pointers to an array of BPQueueElement, and a number which represents the array's size.
 * It copies the elements from the first array to the second array returns true if the queue is empty, else it returns false.
 * If from is NULL, or to is NULL or size is lower than 0 it does nothing.
 * This function assumes that size in the size of the arrays.
 *
 * @param from - the array to copy from.
 * @param to - the array to copy to.
 * @param size - the number of elements in from.
 * @para maxSize - the maximum number of elements in the arrays.
 * @para start - the index in which the first element in from.
 *
 */
void queuecpy(BPQueueElement* from, BPQueueElement* to, int size, int maxSize, int start);

/**
 * This function gets a BPQ, creates a new hard copy of it, and returns it.
 * If source is NULL, or if there is not enough memory to create the copied queue, it returns NULL.
 *
 * @param source - the BPQ to be copied.
 *
 * @return SPBPQueue* for a copy of source or NULL as mentioned above.
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 * A destroy function for BPQ, which frees all its memory resources.
 * If source is NULL it does nothing.
 *
 * @param source - the BPQ to be destroyed.
 *
 */
void spBPQueueDestroy(SPBPQueue* source);

/**
 * This function empties the given BPQ, so there are no elements in it.
 * If source is NULL it does nothing.
 *
 * @param source - the BPQ to be cleared.
 *
 */
void spBPQueueClear(SPBPQueue* source);

/**
 * This function gets a BPQ and returns the number of element in it.
 * If source is NULL it returns -1.
 *
 * @param source - the BPQ to check its size.
 *
 * @return number of elements in source or -1 as mentioned above.
 *
 */
int spBPQueueSize(SPBPQueue* source);

/**
 * This function gets a BPQ and returns its maximum capacity.
 * If source is NULL it returns -1.
 *
 * @param source - the BPQ to check its maximum capacity.
 *
 * @return number of elements source can contain or -1 as mentioned above.
 *
 */
int spBPQueueGetMaxSize(SPBPQueue* source);

/**
 * This function gets a pointer to a BPQ and to indices of two BPQueueElements in the given BPQ, and swaps their place in the queue.
 * It assumes that source is initialized.
 * If source is NULL, or one of the indices is out of the queue's boundaries it does nothing.
 * Otherwise it does as mentioned above.
 *
 * @param source - the BPQ to alter.
 * @param first - the index of the first element.
 * @param second - the index of the second element.
 *
 */
void swapElements(SPBPQueue* source, int first, int second);

/**
 * This function gets a BPQ an index and a value, and decided whether the values should be inserted to the queue.
 * If source is NULL or value is lower than 0 it does nothing to the queue and returns SP_BPQUEUE_INVALID_ARGUMENT message.
 * If the value is greater than any value in the queue, or if it equals to a value but it's index is greater than the equal value's index
 *  it does nothing to the queue and returns SP_BPQUEUE_FULL message.
 * Else it prepares the queue to the insertion and returns SP_BPQUEUE_SUCCESS message.
 *
 * @param source - the BPQ to add the data.
 * @param index - the index to add to the queue.
 * @para value - the value to sort uppon.
 *
 * @return an appropriate message as mentioned above
 *
 */
SP_BPQUEUE_MSG spBPQueueSpaceCheck(SPBPQueue* source, int index, double value);

/**
 * This function gets a BPQ an index and a value, and adds it to the queue if it's possible.
 * If source is NULL or value is lower than 0 it does nothing to the queue and returns SP_BPQUEUE_INVALID_ARGUMENT message.
 * If the queue source is full it does nothing to the queue and returns SP_BPQUEUE_FULL message.
 * Else the function should return SP_BPQUEUE_SUCCESS and do as she supposed to.
 *
 * @param source - the BPQ to add the data.
 * @param index - the index to add to the queue.
 * @para value - the value to sort uppon.
 *
 * @return an appropriate message as mentioned above
 *
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value);

/**
 * This function gets a BPQ, and removes the element with the highest value if it's possible.
 * If source is NULL it does nothing to the queue and returns SP_BPQUEUE_INVALID_ARGUMENT message.
 * If the queue source is empty it does nothing to the queue and returns SP_BPQUEUE_EMPTY message.
 * Else the function should return SP_BPQUEUE_SUCCESS and do as she supposed to.
 *
 * @param source - the BPQ to remove from the element.
 *
 * @return an appropriate message as mentioned above
 *
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source);

/**
 * This function gets a BPQ, and a pointer to a BPQueueElement and copies the data of the element with the lowest value
 * to the given pointer if it's possible.
 * If source is NULL or res is NULL, it does nothing to res and returns SP_BPQUEUE_INVALID_ARGUMENT message.
 * If the queue source is empty it does nothing to res and returns SP_BPQUEUE_EMPTY message.
 * Else the function should return SP_BPQUEUE_SUCCESS and do as she supposed to.
 *
 * @param source - the BPQ to check.
 * @param res - the pointer to copy the data to.
 *
 *
 * @return an appropriate message as mentioned above
 *
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res);

/**
 * This function gets a BPQ, and a pointer to a BPQueueElement and copies the data of the element with the highest value
 * to the given pointer if it's possible.
 * If source is NULL or res is NULL, it does nothing to res and returns SP_BPQUEUE_INVALID_ARGUMENT message.
 * If the queue source is empty it does nothing to res and returns SP_BPQUEUE_EMPTY message.
 * Else the function should return SP_BPQUEUE_SUCCESS and do as she supposed to.
 *
 * @param source - the BPQ to check.
 * @param res - the pointer to copy the data to.
 *
 *
 * @return an appropriate message as mentioned above
 *
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res);

/**
 * This function gets a BPQ, and returns the minimum value in the queue.
 * If source is NULL or the queue is empty, it returns -1.
 *
 * @param source - the BPQ to check.
 *
 * @return the minimum value in source or -1 as mentioned above.
 *
 */
double spBPQueueMinValue(SPBPQueue* source);

/**
 * This function gets a BPQ, and returns the maximum value in the queue.
 * If source is NULL or the queue is empty, it returns -1.
 *
 * @param source - the BPQ to check.
 *
 * @return the maximum value in source or -1 as mentioned above.
 *
 */
double spBPQueueMaxValue(SPBPQueue* source);

/**
 * This function gets a BPQ, and returns true if the queue is empty, else it returns false.
 * If source is NULL, it returns false.
 *
 * @param source - the BPQ to check.
 *
 * @return true/false as mentioned above.
 *
 */
bool spBPQueueIsEmpty(SPBPQueue* source);

/**
 * This function gets a BPQ, and returns true if the queue is full, else it returns false.
 * If source is NULL, it returns false.
 *
 * @param source - the BPQ to check.
 *
 * @return true/false as mentioned above.
 *
 */
bool spBPQueueIsFull(SPBPQueue* source);

#endif
