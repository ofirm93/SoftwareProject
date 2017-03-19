//
// Created by Ofir on 15/03/2017.
//

#include "SPPoint.h"

#ifndef SOFTWAREPROJECT_SPKDARRAY_H
#define SOFTWAREPROJECT_SPKDARRAY_H

typedef struct sp_kd_array_t* SPKDArray;

SPKDArray spInitSPKDArray(SPPoint** pointsArr, int arrSize);

SPKDArray* spSplitSPKDArray(SPKDArray kdArr, int coor);

int getKDArraySize(SPKDArray array);

#endif //SOFTWAREPROJECT_SPKDARRAY_H
