/*
 * SPPoint.c
 *
 *  Created on: Dec 21, 2016
 *      Author: user
 */
#include "SPPoint.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

struct sp_point_t{
	double* data;
	int dim;
	int index;
};

SPPoint* spPointCreate(double* data, int dim, int index){
	if(data == NULL || dim <= 0 || index <0)
	{
		return NULL;
	}
	SPPoint* point = malloc(sizeof(*point));
	if(point == NULL)
	{
		return NULL;
	}
	point->data = (double*) malloc(dim * sizeof(*data));
	for( int i=0 ; i<dim ; i++)
	{
		point->data[i] = data[i];
	}
	point->dim = dim;
	point->index = index;
	return point;
}


SPPoint* spPointCopy(SPPoint* source){
	assert(source != NULL);
	return spPointCreate(source->data, source->dim, source->index);
}

void spPointDestroy(SPPoint* point){
	free(point->data);
	point->data = NULL; //if we dont we crash on test!!!
	free(point);
	point = NULL; //safety on test. might need to remove this when we use valgrind
}

int spPointGetDimension(SPPoint* point){
	assert(point != NULL);
	return point->dim;
}

int spPointGetIndex(SPPoint* point){
	assert(point != NULL);
	return point->index;
}

double spPointGetAxisCoor(SPPoint* point, int axis){
	assert(point != NULL);
	assert(axis >= 0 && axis < point -> dim);
	return point->data[axis];
}
double spPointL2SquaredDistance(SPPoint* p, SPPoint* q){
	double sum = 0;
	assert(p != NULL);
	assert(q != NULL);
	assert(p -> dim == q -> dim);
	for(int i = 0; i < p->dim ; i++){
		sum+= (p->data[i] - q->data[i]) * (p->data[i] - q->data[i]);
	}
	return sum;
}




