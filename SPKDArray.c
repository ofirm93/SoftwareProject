//
// Created by Ofir on 15/03/2017.
//

#include <stdlib.h>
#include <stdbool.h>
#include "SPKDArray.h"
#include "SPBPriorityQueue.h"

struct sp_kd_array_t{
    SPPoint** pointsArray;
    int** sortArray;
    int dimension;
    int numOfPoints;
};

void spDestroyKDArrayWithDim(SPKDArray arr, int lastInitializedDim) {
    if(arr){
        for (int i = 0; i < arr->dimension && i < lastInitializedDim; ++i) {
            free(arr->sortArray[i]);
        }
        for (int i = 0; i < arr->numOfPoints; ++i) {
            spPointDestroy(arr->pointsArray[i]);
        }
        free(arr->sortArray);
        free(arr->pointsArray);
        free(arr);
    }
}

void spDestroyKDArray(SPKDArray arr){
    if (arr){
        spDestroyKDArrayWithDim(arr, arr->dimension);
    }
}

SPKDArray spKDArrayConstructor(int dimension, int size){
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
    if(!(arr->sortArray)){
        free(arr->pointsArray);
        free(arr);
        return NULL;
    }
    for (int i = 0; i < dimension; ++i) {
        arr->sortArray[i] = malloc(size * sizeof(int));
        if(!(arr->sortArray[i])){
            spDestroyKDArrayWithDim(arr, i);
            return NULL;
        }
    }
    return arr;
}

int elementByValueComparator(const void *element1, const void *element2){
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
    SPKDArray array = spKDArrayConstructor(pointDim, arrSize);
    if (!array){
        return NULL;
    }
  
    for (int i = 0; i < arrSize; ++i) { // update to concrete points array
        if(pointsArr[i]){
            array->pointsArray[i] = spPointCopy(pointsArr[i]);
        }
    }

    BPQueueElement* sortingArr = malloc(arrSize * sizeof(BPQueueElement)); // initialize helper array
    if (!sortingArr) {
        spDestroyKDArrayWithDim(array, pointDim);
        return NULL;
    }

    for (int i = 0; i < pointDim; ++i) { // sorting by all coordinates
        for (int j = 0; j < arrSize; ++j) {
            sortingArr[j].index = j;
            sortingArr[j].value = spPointGetAxisCoor(array->pointsArray[j], i);
        }
        qsort(sortingArr, (size_t) arrSize, sizeof(BPQueueElement), elementByValueComparator);
        for (int j = 0; j < arrSize; ++j) {
            array->sortArray[i][j] = sortingArr[j].index;
        }
    }

    free(sortingArr);
    return array;
}

/**
 * TODO write documentation
 * @param kdArr
 * @param coor
 * @return
 */
SPKDArray* spSplitSPKDArray(SPKDArray kdArr, int coor){
    if(!kdArr || coor < 0 || coor >= kdArr->dimension){
        return NULL;
    }
    int size = kdArr->numOfPoints;
    int dim = kdArr->dimension;
    int rightSize = size / 2;
    int leftSize = size - rightSize;
    SPKDArray kdLeft = spKDArrayConstructor(dim, leftSize);
    if(!kdLeft){
        return NULL;
    }
    SPKDArray kdRight = spKDArrayConstructor(dim, rightSize);
    if(!kdRight){
        spDestroyKDArrayWithDim(kdLeft, kdLeft->dimension);
        return NULL;
    }

    bool* isInLeft = malloc(size * sizeof(bool));
    if (!isInLeft){
        spDestroyKDArrayWithDim(kdLeft, dim);
        spDestroyKDArrayWithDim(kdRight, dim);
        return NULL;
    }

    int* newIndices = malloc(size * sizeof(int));
    if (!newIndices){
        free(isInLeft);
        spDestroyKDArrayWithDim(kdLeft, dim);
        spDestroyKDArrayWithDim(kdRight, dim);
        return NULL;
    }

    for (int i = 0; i < leftSize; ++i) {
        int currIndex = kdArr->sortArray[coor][i];
        kdLeft->pointsArray[i] = spPointCopy(kdArr->pointsArray[currIndex]);
        isInLeft[kdArr->sortArray[coor][i]] = true;
        newIndices[currIndex] = i;
    }
    for (int i = leftSize; i < size; ++i) {
        int currIndex = kdArr->sortArray[coor][i];
        kdRight->pointsArray[i - leftSize] = spPointCopy(kdArr->pointsArray[currIndex]);
        isInLeft[kdArr->sortArray[coor][i]] = false;
        newIndices[currIndex] = i - leftSize;
    }

    for (int i = 0; i < dim; ++i) {
        int leftIndex = 0;
        int rightIndex = 0;
        for (int j = 0; j < size; ++j) {
            int index = kdArr->sortArray[i][j];
            if(isInLeft[index]){
                kdLeft->sortArray[i][leftIndex] = newIndices[index];
                leftIndex++;
            }
            else{
                kdRight->sortArray[i][rightIndex] = newIndices[index];
                rightIndex++;
            }
        }
    }
    free(newIndices);
    free(isInLeft);
    SPKDArray* twoKDArrays = malloc(2 * sizeof(SPKDArray));
    if (!twoKDArrays){
        spDestroyKDArrayWithDim(kdLeft, dim);
        spDestroyKDArrayWithDim(kdRight, dim);
        return NULL;
    }
    twoKDArrays[0] = kdLeft;
    twoKDArrays[1] = kdRight;
    return twoKDArrays;
}

SPPoint* spGetSPKDArrayPoint(SPKDArray kdArr, int index){
    if(!kdArr || index < 0 || index >= kdArr->numOfPoints || !(kdArr->pointsArray)){
        return NULL;
    }
    return spPointCopy(kdArr->pointsArray[index]);
}

int spGetSPKDArraySize(SPKDArray kdArr){
    if(!kdArr){
        return 0;
    }
    return kdArr->numOfPoints;
}

// TODO Delete the next method - its for testing only
#include <stdio.h>
char* spPrintKDArrayDetails(SPKDArray kdArr){
    if(kdArr){
        int size = kdArr->numOfPoints;
        int dim = kdArr->dimension;
        char* msg = malloc(10000 * sizeof(char));
        if(!msg){
            return "Allocation Error";
        }
        sprintf(msg, "The size of the array is : %d\n"
                "The dimension of the array is : %d\n"
                "The points in the array are :\n", size, dim);
        for (int i = 0; i < size; ++i) {
            sprintf(msg, "%spoint %2d : (", msg,i);
            for (int j = 0; j < dim; ++j) {
                if(j > 0){
                    sprintf(msg, "%s, ", msg);
                }
                sprintf(msg, "%s%2lf", msg,spPointGetAxisCoor(kdArr->pointsArray[i], j));
            }
            sprintf(msg, "%s)\n", msg);
        }
        sprintf(msg, "%sThe sorted indices in the array are :\n", msg);
        for (int i = 0; i < dim; ++i) {
            sprintf(msg, "%sdimension %2d : (", msg, i);
            for (int j = 0; j < size; ++j) {
                if(j > 0){
                    sprintf(msg, "%s, ", msg);
                }
                sprintf(msg, "%s%2d", msg, kdArr->sortArray[i][j]);
            }
            sprintf(msg, "%s)\n", msg);
        }
        return msg;
    }
    return "Debug : The argument kdArr is NULL.";
}

// TODO Delete the next method - its for testing only
#include <stdio.h>
