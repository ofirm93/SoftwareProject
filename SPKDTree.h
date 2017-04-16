/*
 * SPKDTree.h
 *
 *  Created on: Mar 18, 2017
 *      Author: user
 */

#ifndef SPKDTREE_H_
#define SPKDTREE_H_

#include "SPKDArray.h"
#include "SPConfig.h"
#include "SPBPriorityQueue.h"
//SPKDArray - pointer so remember to free

//TODO delete after we set system var

typedef struct sp_kd_tree_t SPKDTree;

typedef struct sp_kd_tree_node_t KDTreeNode;




/**
 * *******************
 * KD TREE NODE
 *
 * *******************
 */

/**
 *
 */
KDTreeNode* spInitKDTreeNode(int dim, double val, KDTreeNode* left, KDTreeNode* right, SPPoint* data);

bool isLeaf(KDTreeNode* node);




int spKDTreeNodeGetDim(KDTreeNode* node);

double spKDTreeNodeGetVal(KDTreeNode* node);

KDTreeNode* spKDTreeNodeGetLeft(KDTreeNode* node);

KDTreeNode* spKDTreeNodeGetRight(KDTreeNode* node);

SPPoint* spKDTreeNodeGetPoint(KDTreeNode* node);

int spKDTreeNodeGetPointIndex(KDTreeNode* node);


double spKDTreeNodeGetPointVal(KDTreeNode* node);


void spDestroyKDTreeNode(KDTreeNode* node);



/**
 * *******************
 * KD TREE
 *
 * *******************
 */

/**
 * A constructor for the KDTree.
 *
 * @param
 * arr - the array of points that we want to create the tree for
 * size - how many points are in arr
 * pointDim - the dimension of the points
 * splitMethod - how to split the array within the tree while building it.
 *
 * @return
 * A pointer to the tree on success.
 * Otherwise,NULL.
 */
SPKDTree* spInitSPKDTree(SPPoint** arr, int size, int pointDim, SP_KD_SPLIT_MODE splitMethod);

/**
 * Returns the root of the tree.
 *
 * @param
 * kdTree- the tree which we want to get the root for
 *
 * @return
 * A pointer to the tree node which is the root on success.
 * Otherwise, NULL;
 */
KDTreeNode* spGetSPKDTreeRoot(SPKDTree* tree);


#endif /* SPKDTREE_H_ */
