//
// Created by Ofir on 15/03/2017.
//

#include <stdlib.h>
#include <stdbool.h>
#include "SPKDArray.h"
#include "SPBPriorityQueue.h"

int coordinate = 0;

struct sp_kd_array_t{
    SPPoint** pointsArray;
    int** sortArray;
    int dimension;
    int numOfPoints;
};

void spDestroyKDArray(SPKDArray arr, int lastInitializedDim) {
    for (int i = 0; i < lastInitializedDim; ++i) {
        free(arr->sortArray[i]);
    }
    for (int i = 0; i < arr->numOfPoints; ++i) {
        spPointDestroy(arr->pointsArray[i]);
    }
    free(arr->sortArray);
    free(arr->pointsArray);
    free(arr);
}

SPKDArray spkdArrayConstructor(int dimension, int size){
    if(dimension < 1 || size < 1){
        return NULL;
    }
    SPKDArray arr = malloc(sizeof(struct sp_kd_array_t));
    if(!arr){
        return NULL;
    }
    arr->dimension = dimension;
    arr->numOfPoints = size;
    arr->pointsArray = malloc(size * sizeof(SPPoint*));
    if(!(arr->pointsArray)){
        free(arr);
        return NULL;
    }
    arr->sortArray = malloc(dimension * sizeof(int*));
    if(!arr->sortArray){
        free(arr->pointsArray);
        free(arr);
        return NULL;
    }
    for (int i = 0; i < dimension; ++i) {
        arr->sortArray[i] = malloc(size * sizeof(int));
        if(!(arr->sortArray[i])){
            spDestroyKDArray(arr, i);
            return NULL;
        }
    }
    return arr;
}

int comparator(const void* element1, const void* element2){
    double value1 = (*(BPQueueElement*)element1).value;
    double value2 = (*(BPQueueElement*)element2).value;
    if(value1 - value2 == 0){
        return 0;
    }
    else if( value1 - value2 > 0){
        return 1;
    }
    return -1;
}

SPKDArray spInitSPKDArray(SPPoint** pointsArr, int arrSize){
    if(!pointsArr || arrSize < 1){ // input check
        return NULL;
    }
    int pointDim = spPointGetDimension(pointsArr[0]); // basic KDArray initialization
    SPKDArray array = spkdArrayConstructor(pointDim, arrSize);
    if (!array){
        return NULL;
    }
  
    for (int i = 0; i < arrSize; ++i) { // update to concrete points array
        array->pointsArray[i] = spPointCopy(pointsArr[i]);
    }

    BPQueueElement* sortingArr = malloc(arrSize * sizeof(BPQueueElement)); // initialize helper array
    if (!sortingArr) {
        spDestroyKDArray(array, pointDim);
        return NULL;
    }

    for (int i = 0; i < pointDim; ++i) { // sorting by all coordinates
        for (int j = 0; j < arrSize; ++j) {
            sortingArr[j].index = j;
            sortingArr[j].value = spPointGetAxisCoor(array->pointsArray[j], i);
        }
        qsort(sortingArr, (size_t) arrSize, sizeof(BPQueueElement), comparator);
        for (int j = 0; j < arrSize; ++j) {
            array->sortArray[i][j] = sortingArr[j].index;
        }
    }

    free(sortingArr);
    return array;
}

/**
 * TODO improve this method
 * @param kdArr
 * @param coor
 * @param kdLeft
 * @param kdRight
 * @return
 */
bool spSplitSPKDArray(SPKDArray kdArr, int coor, SPKDArray* kdLeft, SPKDArray* kdRight){
    if(!kdArr || coor < 0 || !kdLeft || !kdRight){
        return false;
    }
    int size = kdArr->numOfPoints;
    int rightSize = size / 2;
    int leftSize = size - rightSize;
    *kdLeft = spInitSPKDArray(kdArr->array[coor], leftSize);
    if(!(*kdLeft)){
        return false;
    }
    *kdRight = spInitSPKDArray((kdArr->array[coor]) + leftSize, rightSize);
    if(!(*kdRight)){
        spDestroyKDArray(*kdLeft, (*kdLeft)->numOfPoints);
        return false;
    }
    return true;
}
