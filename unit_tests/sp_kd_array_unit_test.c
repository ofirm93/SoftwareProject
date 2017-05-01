//
// Created by Ofir on 22/03/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../SPKDArray.h"
#include "../SPLogger.h"
#include "unit_test_util.h"

#define ERR_MSG_CANNOT_ALLOCATE_MEM "Error : Couldn't allocate needed memory."
#define ERR_MSG_UNEXPECTED_VALUE "Error : The value retrieved after the split is different from the expected one."
#define ERR_MSG_FAILED_TO_SPLIT "Error : Failed splitting the array."
#define ERR_MSG_FAIL_TO_INIT "Error : Failed initializing the array."

#define NUM_OF_TEST_CASES 2
#define DIMENSION 3
#define NUM_OF_POINTS 5

/**
 * Tester for splitting a given array.
 * @param testArray
 * @param numOfPoints
 * @param dimension
 * @param expectedArray
 * @return true if successful otherwise false
 */
bool spTestSplitForArray(double testArray[NUM_OF_POINTS][DIMENSION], int numOfPoints, int dimension, double expectedArray[DIMENSION][NUM_OF_POINTS][DIMENSION]){
    SPPoint** pointArr = malloc(numOfPoints * sizeof(SPPoint*));
    if(!pointArr){
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        return false;
    }

    for (int j = 0; j < numOfPoints; ++j) {
        pointArr[j] = spPointCreate(testArray[j], dimension , j);
    }
    SPKDArray kdArr = spInitSPKDArray(pointArr, numOfPoints);
    if(!kdArr){
        spLoggerPrintError(ERR_MSG_FAIL_TO_INIT, __FILE__, __func__, __LINE__);
        for (int l = 0; l < numOfPoints; ++l) {
            spPointDestroy(pointArr[l]);
        }
        free(pointArr);
        return false;
    }
    char mainTitle[1024] = "The big array's details are :\n";
    char* mainMsg = spPrintKDArrayDetails(kdArr);
    for (int k = 0; k < dimension; ++k) {
        SPKDArray* twoArrays = spSplitSPKDArray(kdArr, k);
        if(!twoArrays){
            spLoggerPrintError(ERR_MSG_FAILED_TO_SPLIT, __FILE__, __func__, __LINE__);
            spDestroyKDArray(kdArr);
            for (int l = 0; l < numOfPoints; ++l) {
                spPointDestroy(pointArr[l]);
            }
            free(pointArr);
            return false;
        }

        char leftTitle[1024] = "\n\nThe left array's details are :\n";
        char* leftMsg = spPrintKDArrayDetails(twoArrays[0]);
        char rightTitle[1024] = "\n\nThe right array's details are :\n";
        char* rightMsg = spPrintKDArrayDetails(twoArrays[1]);
        char msg[20000];
        sprintf(msg, "%s%s%s%s%s%s", mainTitle, mainMsg, leftTitle, leftMsg, rightTitle, rightMsg);
        spLoggerPrintDebug(msg, __FILE__, __func__, __LINE__);

        for (int l = 0; l < dimension; ++l) {
            for (int point = 0; point < numOfPoints - numOfPoints/2; ++point) {
                SPPoint* p = spGetSPKDArrayPoint(twoArrays[0], point);
                for (int dim = 0; dim < dimension; ++dim) {
                    double expectedValue = expectedArray[k][point][dim];
                    double value = spPointGetAxisCoor(p, dim);
                    if(expectedValue != value){
                        spLoggerPrintError(ERR_MSG_UNEXPECTED_VALUE, __FILE__, __func__, __LINE__);
                        spDestroyKDArray(kdArr);
                        spDestroyKDArray(twoArrays[0]);
                        spDestroyKDArray(twoArrays[1]);
                        free(twoArrays);
                        for (int deletePoint = 0; deletePoint < numOfPoints; ++deletePoint) {
                            spPointDestroy(pointArr[deletePoint]);
                        }
                        free(pointArr);
                        free(testArray);
                        free(expectedArray);
                        return false;
                    }
                }
            }
            for (int point = numOfPoints - numOfPoints/2; point < numOfPoints; ++point) {
                SPPoint* p = spGetSPKDArrayPoint(twoArrays[1], point - (numOfPoints - numOfPoints/2));
                for (int dim = 0; dim < dimension; ++dim) {
                    double expectedValue = expectedArray[k][point][dim];
                    double value = spPointGetAxisCoor(p, dim);
                    if(expectedValue != value){
                        spLoggerPrintError(ERR_MSG_UNEXPECTED_VALUE, __FILE__, __func__, __LINE__);
                        spDestroyKDArray(kdArr);
                        spDestroyKDArray(twoArrays[0]);
                        spDestroyKDArray(twoArrays[1]);
                        free(twoArrays);
                        for (int deletePoint = 0; deletePoint < numOfPoints; ++deletePoint) {
                            spPointDestroy(pointArr[deletePoint]);
                        }
                        free(pointArr);
                        free(testArray);
                        free(expectedArray);
                        return false;
                    }

                }
            }
        }

        spDestroyKDArray(twoArrays[0]);
        spDestroyKDArray(twoArrays[1]);
        free(twoArrays);
    }
    for (int k = 0; k < numOfPoints; ++k) {
        spPointDestroy(pointArr[k]);
    }
    spDestroyKDArray(kdArr);
    free(pointArr);
    return true;
}
/**
 * A tester for splitting arrays. based on edge cases.
 * @return true if successful otherwise false
 */
bool testSplitSPKDArray(){
    int numOfPoints[NUM_OF_TEST_CASES] = {5, 4};
    double testArr1[NUM_OF_POINTS][DIMENSION] = {{0, 5, -5}, {1, 4, -4}, {2, 3, -3}, {3, 2, -2}, {4, 1, -1}};
    double testArr2[NUM_OF_POINTS][DIMENSION] = {{0, 5, -5}, {1, 4, -4},  {3, 2, -2}, {4, 1, -1}, {0, 0, 0}};

    double expectedArr1[DIMENSION][NUM_OF_POINTS][DIMENSION] = {
            {{0, 5, -5}, {1, 4, -4}, {2, 3, -3}, {3, 2, -2}, {4, 1, -1}},
            {{4, 1, -1}, {3, 2, -2}, {2, 3, -3}, {1, 4, -4}, {0, 5, -5}},
            {{0, 5, -5}, {1, 4, -4}, {2, 3, -3}, {3, 2, -2}, {4, 1, -1}}
    };
    double expectedArr2[DIMENSION][NUM_OF_POINTS][DIMENSION] = {
            {{0, 5, -5}, {1, 4, -4}, {3, 2, -2}, {4, 1, -1}, {0, 0, 0}},
            {{4, 1, -1}, {3, 2, -2}, {1, 4, -4}, {0, 5, -5}, {0, 0, 0}},
            {{0, 5, -5}, {1, 4, -4}, {3, 2, -2}, {4, 1, -1}, {0, 0, 0}}
    };
    bool f = spTestSplitForArray(testArr1, numOfPoints[0], DIMENSION, expectedArr1);
    if(!f){
        return false;
    }
    f = spTestSplitForArray(testArr2, numOfPoints[1], DIMENSION, expectedArr2);
    if(!f){
        return false;
    }
    return true;
}

/*
int main() {
    SP_LOGGER_MSG loggerMsg = spLoggerCreate(NULL, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);
    if(loggerMsg != SP_LOGGER_SUCCESS){
        printf("Failed creating the logger.");
        return 1;
    }
    RUN_TEST( testSplitSPKDArray );
    spLoggerDestroy();
    return 0;
}
*/
