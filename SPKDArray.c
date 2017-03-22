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
    SPKDArray array = spKDArrayConstructor(pointDim, arrSize);
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
        spDestroyKDArray(kdLeft, kdLeft->dimension);
        return NULL;
    }

    bool* isInLeft = malloc(size * sizeof(bool));
    if (!isInLeft){
        spDestroyKDArray(kdLeft, dim);
        spDestroyKDArray(kdRight, dim);
        return NULL;
    }

    for (int i = 0; i < leftSize; ++i) {
        kdLeft->pointsArray[i] = kdArr->pointsArray[kdArr->sortArray[coor][i]];
        isInLeft[kdArr->sortArray[coor][i]] = true;
    }
    for (int i = leftSize; i < size; ++i) {
        kdRight->pointsArray[i - leftSize] = kdArr->pointsArray[kdArr->sortArray[coor][i]];
        isInLeft[kdArr->sortArray[coor][i]] = false;
    }

    for (int i = 0; i < dim; ++i) {
        int leftIndex = 0;
        int rightIndex = 0;
        for (int j = 0; j < size; ++j) {
            int index = kdArr->sortArray[i][j];
            if(isInLeft[index]){
                kdLeft->sortArray[i][leftIndex] = index;
                leftIndex++;
            }
            else{
                kdRight->sortArray[i][rightIndex] = index;
                rightIndex++;
            }
        }
    }

    free(isInLeft);
    SPKDArray* twoKDArrays = malloc(2 * sizeof(SPKDArray));
    twoKDArrays[0] = kdLeft;
    twoKDArrays[1] = kdRight;
    return twoKDArrays;
}

SPPoint* spGetSPKDArrayPoint(SPKDArray kdArr, int index){
    if(!kdArr || index < 0 || index >= kdArr->numOfPoints || !(kdArr->pointsArray)){
        return NULL;
    }
    return kdArr->pointsArray[index];
}

int spGetSPKDArraySize(SPKDArray kdArr){
    if(!kdArr){
        return 0;
    }
    return kdArr->numOfPoints;
}

// TODO Delete the next method - its for testing only
#include <stdio.h>
void spPrintKDArrayDetails(SPKDArray kdArr){
    if(kdArr){
        int size = kdArr->numOfPoints;
        int dim = kdArr->dimension;
        printf("The size of the array is : %d\n", size);
        printf("The dimension of the array is : %d\n", dim);
        printf("The points in the array are :\n");
        for (int i = 0; i < size; ++i) {
            printf("point %2d : (", i);
            for (int j = 0; j < dim; ++j) {
                if(j > 0){
                    printf(", ");
                }
                printf("%2lf", spPointGetAxisCoor(kdArr->pointsArray[i], j));
            }
            printf(")\n");
        }
        printf("The sorted indices in the array are :\n");
        for (int i = 0; i < dim; ++i) {
            printf("dimension %2d : (", i);
            for (int j = 0; j < size; ++j) {
                if(j > 0){
                    printf(", ");
                }
                printf("%2d", kdArr->sortArray[i][j]);
            }
            printf(")\n");
        }
    }
}

