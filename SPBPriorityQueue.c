/*
 * SPBPriorityQueue.c
 *
 *  Created on: 22 ����� 2016
 *      Author: Ofir
 */
#include <stdlib.h>
#include "SPBPriorityQueue.h"

struct sp_bp_queue_t{
	int maxSize;
	int start;
	int end;
	int size;
	BPQueueElement* queue;
};


SPBPQueue* spBPQueueCreate(int maxSize){
	SPBPQueue* newQueue = NULL;
	if(maxSize < 0){
		return NULL;
	}
	newQueue = (SPBPQueue*) malloc(sizeof(SPBPQueue));
	if(newQueue == NULL){
		return NULL;
	}
	newQueue->maxSize = maxSize;
	newQueue->size = 0;
	newQueue->start = 0;
	newQueue->end = 0;
	newQueue->queue = (BPQueueElement*) malloc(maxSize*sizeof(BPQueueElement));
	if(newQueue->queue == NULL){
		return NULL;
	}
	return newQueue;
}

void queuecpy(BPQueueElement* from, BPQueueElement* to, int size, int maxSize, int start){
	if(from == NULL || to == NULL || size <=0 || maxSize <= 0 || start <0){
		return;
	}
	for(int i = 0; i < size; i++){
		to[(start+i)%maxSize].index = from[(start+i)%maxSize].index;
		to[(start+i)%maxSize].value = from[(start+i)%maxSize].value;
	}
}

SPBPQueue* spBPQueueCopy(SPBPQueue* source){
	if(source == NULL){
		return NULL;
	}
	SPBPQueue* copy = spBPQueueCreate(source->maxSize);
	if(copy == NULL){
		return NULL;
	}
	copy->size = source->size;
	copy->start = source->start;
	copy->end = source->end;
	queuecpy(source->queue, copy->queue, source->size, source->maxSize, source->start);
	return copy;
}


void spBPQueueDestroy(SPBPQueue* source){
	if(source != NULL){
		free(source->queue);
		free(source);
	}
}


void spBPQueueClear(SPBPQueue* source){
	if(source != NULL){
		source->size = 0;
		source->start = 0;
		source->end = 0;
	}
}


int spBPQueueSize(SPBPQueue* source){
	if(source == NULL){
		return -1;
	}
	return source->size;
}


int spBPQueueGetMaxSize(SPBPQueue* source){
	if(source == NULL){
		return -1;
	}
	return source->maxSize;
}

void swapElements(SPBPQueue* source, int first, int second){
	if(source == NULL){
		return;
	}
	if(first < 0 || first > source->maxSize){
		return;
	}
	if(second < 0 || second > source->maxSize){
		return;
	}
	int tempIndex = source->queue[first].index;
	double tempValue = source->queue[first].value;
	source->queue[first].index = source->queue[second].index;
	source->queue[first].value = source->queue[second].value;
	source->queue[second].index = tempIndex;
	source->queue[second].value = tempValue;
}

SP_BPQUEUE_MSG spBPQueueSpaceCheck(SPBPQueue* source, int index, double value){
	BPQueueElement* last = malloc(sizeof(BPQueueElement));
	if(source == NULL || value < 0 || last == NULL){
		free(last);
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(source->maxSize <= source->size){
		if(spBPQueuePeekLast(source, last) == SP_BPQUEUE_SUCCESS){
			if(last->value <= value){
				if(last->value < value){
					free(last);
					return SP_BPQUEUE_FULL;
				}
				if(last->index <= index){
					free(last);
					return SP_BPQUEUE_FULL;
				}
			}
			if(source->maxSize == 0){
				free(last);
				return SP_BPQUEUE_FULL;
			}
			source->end--;
			if(source->end < 0){ // the array is cyclic so if end is out of bounds it returns to the beginning.
				source->end = source->maxSize - 1;
			}
			source->size--;
		}
	}
	free(last);
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value){
	bool isOrdered;
	int curr;
	int prev;

	if(source == NULL || value < 0){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

	SP_BPQUEUE_MSG msg = spBPQueueSpaceCheck(source, index, value);
	if(msg != SP_BPQUEUE_SUCCESS){
		return msg;
	}
	source->queue[source->end].index = index;
	source->queue[source->end].value = value;
	source->size++;
	source->end++;
	if(source->end >= source->maxSize){ // the array is cyclic so if end is out of bounds it returns to the beginning.
		source->end = 0;
	}

	isOrdered = false;
	curr = source->end;
	for(int i = 0; i < source->size - 1 && !isOrdered; i++){  //this for loop continues until it ended all size -1 iterations or if the queue is sorted
		curr--;
		if(curr < 0){
			curr += source->maxSize;
		}
		prev = curr - 1;
		if(prev < 0){
			prev += source->maxSize;
		}
		if(source->queue[curr].value <= source->queue[prev].value){
			if(source->queue[curr].value < source->queue[prev].value){
				swapElements(source, prev, curr);
			}
			else{
				if(source->queue[curr].index < source->queue[prev].index){
					swapElements(source, prev, curr);
				}
				else{
					isOrdered = true;
				}
			}
		}
		else{
			isOrdered = true;
		}
	}

	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source){
	if(source == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(source->size <= 0){
		return SP_BPQUEUE_EMPTY;
	}
	source->start++;
	if(source->start >= source->maxSize){
		source->start = 0;
	}
	source->size--;
	return SP_BPQUEUE_SUCCESS;
}


SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res){
	if(source == NULL || res == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(source->size <= 0){
		return SP_BPQUEUE_EMPTY;
	}
	res->index = source->queue[source->start].index;
	res->value = source->queue[source->start].value;
	return SP_BPQUEUE_SUCCESS;
}


SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res){
	int position;
	if(source == NULL || res == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(source->size <= 0){
		return SP_BPQUEUE_EMPTY;
	}
	position = source->end - 1;
	if(position < 0){ // if out of bounds return to the end of the array
		position = source->maxSize - 1;
	}
	res->index = source->queue[position].index;
	res->value = source->queue[position].value;
	return SP_BPQUEUE_SUCCESS;
}


double spBPQueueMinValue(SPBPQueue* source){
	if(source == NULL || source->size <= 0){
		return -1;
	}
	return source->queue[source->start].value;
}

double spBPQueueMaxValue(SPBPQueue* source){
	int position;
	if(source == NULL || source->size <= 0){
		return -1;
	}
	position = source->end - 1;
	if(position < 0){ // if out of bounds return to the end of the array
		position = source->maxSize - 1;
	}
	return source->queue[position].value;
}


bool spBPQueueIsEmpty(SPBPQueue* source){
	if(source == NULL){
		return false;
	}
	return source->size == 0 ? true : false;
}


bool spBPQueueIsFull(SPBPQueue* source){
	if(source == NULL){
		return false;
	}
	return source->size == source->maxSize ? true : false;
}


