//
// Created by Ofir on 15/03/2017.
//

#include <stdlib.h>
#include <stdbool.h>
#include "SPKDArray.h"

struct sp_kd_array_t{
    SPPoint*** array;
    int dimension;
    int size;
};

void spDestroyKDArray(const struct sp_kd_array_t *arr, int i) {
    for (int j = 0; j < i; j++) {
        free(arr->array[j]);
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
    arr->size = size;
    arr->array = malloc(dimension * sizeof(SPPoint**));
    if(!(arr->array)){
        free(arr);
        return NULL;
    }
    for(int i = 0; i < dimension; i++){
        arr->array[i] = malloc(size * sizeof(SPPoint*));
        if(!(arr->array[i])){
            spDestroyKDArray(arr, i);
            return NULL;
        }
    }
    return arr;
}

SPKDArray spInitSPKDArray(SPPoint** arr, int size){
    if(!arr || size < 1){
        return NULL;
    }
    int dimension = spPointGetDimension(arr[0]);
    SPKDArray array = spkdArrayConstructor(dimension, size);
    if (!array){
        return NULL;
    }
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < size; ++j) {
            array->array[i][j] = arr[j];
        }

        int comparator(const void* point1, const void* point2){
            double coor1 = spPointGetAxisCoor((SPPoint*)point1, i);
            double coor2 = spPointGetAxisCoor((SPPoint*)point2, i);
            if(coor1 - coor2 == 0){
                return 0;
            }
            else if( coor1 - coor2 > 0){
                return 1;
            }
            return -1;
        }

        qsort(array->array[i], (size_t) size, sizeof(SPPoint*), comparator);
    }
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
    int size = kdArr->size;
    int rightSize = size / 2;
    int leftSize = size - rightSize;
    *kdLeft = spInitSPKDArray(kdArr->array[coor], leftSize);
    if(!(*kdLeft)){
        return false;
    }
    *kdRight = spInitSPKDArray((kdArr->array[coor]) + leftSize, rightSize);
    return true;
}
