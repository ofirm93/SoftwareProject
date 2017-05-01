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


/**
 * Created a new KDTreeNode with the arguments given as it's paramaters.
 * @param
 * dim - the spliiting dimension
 * val - the median value of splitting
 * left - the left child of the node
 * right - the right child of the node
 * data - a point associated with the node on a leaf, otherwise is NULL.
 * @return
 * a node with the matching parameters, or NULL if point is NULL & leaf or point isnt NULL & not a leaf.
 *
 */

KDTreeNode* spInitKDTreeNode(int dim, double val, KDTreeNode* left, KDTreeNode* right, SPPoint* data);

/**
 * Gets a node and return true iff
 * the node is a leaf.
 * @param
 * node - the node to check if a leaf
 * @return
 * true if node is a leaf.
 * Else, false.
 */
bool isLeaf(KDTreeNode* node);

/**
 * Gets a node and return the node's
 * dimension - the dimension we split the array
 * in upon descending from the node.
 * @param
 * node - the node to get the dimension of
 * @return
 * the dimension described above, or -1 on failure.
 */
int spKDTreeNodeGetDim(KDTreeNode* node);

/**
 * Gets a node and return the node's
 * value - the median value we split the array by in the dimension
 * of the node.
 * @param
 * node - the node to get the value of.
 * @return
 * the value described above, or -1 on failure.
 */
double spKDTreeNodeGetVal(KDTreeNode* node);
/**
 * Gets a node and returns the left child
 * of the node.
 * @param
 * node - the node to get the child from.
 * @return
 * a pointer to the child described above, or NULL if it doesn't exist or node is NULL;
 */

KDTreeNode* spKDTreeNodeGetLeft(KDTreeNode* node);

/**
 * Gets a node and returns the right child
 * of the node.
 * @param
 * node - the node to get the child from.
 * @return
 * a pointer to the child described above, or NULL if it doesn't exist or node is NULL;
 */

KDTreeNode* spKDTreeNodeGetRight(KDTreeNode* node);
/**
 * Gets the point associated with the node if it is a leaf,
 * otherwise , returns NULL
 * @param
 * node - the node we want to get the point from.
 * @return
 * The point associated with the node if it is a leaf. Otherwise NULL.
 *
 */
SPPoint* spKDTreeNodeGetPoint(KDTreeNode* node);

/**
 * Gets the index of the point associated with the node if it is a leaf,
 * otherwise , returns NULL
 * @param
 * node - the node we want to get the point from.
 * @return
 * The point associated with the node if it is a leaf. Otherwise NULL.
 *
 */
int spKDTreeNodeGetPointIndex(KDTreeNode* node);

/**
 * Gets the value of the point associated with the node if it is a leaf,
 * otherwise , returns NULL
 * @param
 * node - the node we want to get the point from.
 * @return
 * The point associated with the node if it is a leaf. Otherwise NULL.
 *
 */

//double spKDTreeNodeGetPointVal(KDTreeNode* node); //todo delte if not needed

/**
 * frees all memory associated with node.
 * @param
 * node - the node to free the memory of.
 *
 *
 */
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

/**
 * Frees all space allocated for tree.
 * @param
 * tree - the tree to free the space from.
 *
 *
 */
void spDestroyKDTree(SPKDTree* tree);

/**
 * A function to print the tree
 * in order to the LOGGER. helps with testing.
 * @param
 * tree - the tree to scan in order.
 *
 *
 */
void inOrderScan(SPKDTree* tree);

#endif /* SPKDTREE_H_ */
