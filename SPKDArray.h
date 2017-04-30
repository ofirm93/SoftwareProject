//
// Created by Ofir on 15/03/2017.
//

#include "SPPoint.h"

#ifndef SOFTWAREPROJECT_SPKDARRAY_H
#define SOFTWAREPROJECT_SPKDARRAY_H

typedef struct sp_kd_array_t* SPKDArray;

/**
 * This method gets an array of pointers of SPPoint's and its size, initializes an SPKDArray based on the given data,
 * and returns it.
 * @param arr - an array of points to create KD-Array from
 * @param size - the size of the array above
 * @return
 * NULL                     - if one of the arguments is fault or id an error occurs during the initialization
 *                          (memory allocation and so)
 * The initializd KD-Array  - otherwise
 */
SPKDArray spInitSPKDArray(SPPoint** arr, int size);

/**
 * A destroyer method for SPKDArray.
 * @param arr - the KD-Array to destroy
 */
void spDestroyKDArray(SPKDArray arr);

/**
 * This method gets a SPKDArray and a coordinate and splits the KD-Array into two KD-Arrays based on the coordinates.
 * @param kdArr - a SPKDArray to split
 * @param coor - a coordinate to split upon
 * @return
 * NULL                 - if kdArr is NULL or coor is not a valid coordinate or if an error occurs during the split
 * An Array of size 2 of the KD-Arrays pointers after the split - otherwise.
 */
SPKDArray* spSplitSPKDArray(SPKDArray kdArr, int coor);

/**
 * Getter method to a specific point in the array with the given index.
 * @param kdArr - the KD-Array
 * @param index - the index of the requested point
 * @return
 * NULL                 - if kdArr is NULL or index is not valid or if an error occurs during the method.
 * The requested point  - otherwise
 */
SPPoint* spGetSPKDArrayPoint(SPKDArray kdArr, int index);

/**
 * Getter method to the KD-Array size.
 * @param kdArr - the KD-Array
 * @return
 * 0                    - if kdArr is NULL
 * the KD-Array size    - otherwise
 */
int spGetSPKDArraySize(SPKDArray kdArr);


// TODO Delete the next method - its for testing only
/**
 * This method returns a summary about the given array or an error text if it is not valid.
 * @param kdArr - the KD-Array
 * @return
 * Error message    - if kdArray is NULL
 * A summary        - otherwise
 */
char* spPrintKDArrayDetails(SPKDArray kdArr);

#endif //SOFTWAREPROJECT_SPKDARRAY_H