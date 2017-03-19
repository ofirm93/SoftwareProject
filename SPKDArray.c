//
// Created by Ofir on 15/03/2017.
//

#include <stdlib.h>
#include <stdbool.h>
#include "SPKDArray.h"

int coordinate = 0;

struct sp_kd_array_t{
    SPPoint** pointsArray;
    int** sortArray;
    int dimension;
    int numOfPoints;
};

void spDestroyKDArray(SPKDArray arr, int i) {
    for (int j = 0; j < i; j++) {
        free(arr->array[j]); //TODO leftovers from last file, change name of array
    }
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
            for (int j = 0; j < i; ++j) {
                free(arr->sortArray[j]);
            }
            free(arr->sortArray);
            free(arr->pointsArray);
            free(arr);
            return NULL;
        }
    }
    return arr;
}

int comparator(const void* point1, const void* point2){
    double value1 = spPointGetAxisCoor((SPPoint*)point1, coordinate);
    double value2 = spPointGetAxisCoor((SPPoint*)point2, coordinate);
    if(value1 - value2 == 0){
        return 0;
    }
    else if( value1 - value2 > 0){
        return 1;
    }
    return -1;
}

SPKDArray spInitSPKDArray(SPPoint** pointsArr, int arrSize){
    if(!pointsArr || arrSize < 1){
        return NULL;
    }
    int pointDim = spPointGetDimension(pointsArr[0]);
    SPKDArray array = spkdArrayConstructor(pointDim, arrSize);
    if (!array){
        return NULL;
    }
    for (int i = 0; i < arrSize; ++i) {
        array->pointsArray[i] = pointsArr[i]; // TODO Create new point with spPointCreate(pointsArr[j]->dim ...)
    }
    // TODO start editing from here
    for (int i = 0; i < pointDim; ++i) {
        for (int j = 0; j < arrSize; ++j) {
            array->array[i][j] = pointsArr[j]; //TODO If you change the above change here to point to the new point
        }
        coordinate = i;
        qsort(array->array[i], (size_t) arrSize, sizeof(SPPoint*), comparator);
    }
    coordinate = 0;
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
