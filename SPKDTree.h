/*
 * SPKDTree.h
 *
 *  Created on: Mar 18, 2017
 *      Author: user
 */

#ifndef SPKDTREE_H_
#define SPKDTREE_H_

#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPConfig.h"
//SPKDArray - pointer so remember to free

//TODO delete after we set system var

typedef struct sp_kd_tree_t SPKDTree;

typedef struct sp_kd_tree_node_t KDTreeNode;


SPKDTree* spInitSPKDTree(SPPoint** arr, int size, int pointDim, SP_KD_SPLIT_MODE splitMethod);






#endif /* SPKDTREE_H_ */
