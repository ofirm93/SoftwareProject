/*
 * SPKDTree.c
 *
 *  Created on: Mar 18, 2017
 *      Author: user
 */



#include "SPKDTree.h"
//#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>


#define INVALID DBL_MAX
#define MAX_ERR_MSG_LENGTH 1024

#define ERR_MSG_INVALID_ARG "Error : One of the arguments supplied to the method is invalid."


struct sp_kd_tree_node_t
{
	int dim;
	double val;

    KDTreeNode* left;
	KDTreeNode* right;
	SPPoint* data;
};

struct sp_kd_tree_t{
	KDTreeNode* root;
	SPKDArray features;
};


/**
 * *******************
 * KD TREE NODE
 *
 * *******************
 */
KDTreeNode* spInitKDTreeNode(int dim, double val, KDTreeNode* left, KDTreeNode* right, SPPoint* data){
	KDTreeNode* kdNode = malloc(sizeof(KDTreeNode));
	if(!kdNode){
		return NULL;
	}
	kdNode -> dim = dim;
	kdNode -> val = val;
	kdNode -> left = left;
	kdNode -> right = right;
	if(!left && !right && !data)
	{
		spDestroyKDTreeNode(kdNode);
		spLoggerPrintError("Error : Point shouldn't be NULL if it is a leaf.", __FILE__, __func__, __LINE__);
		//printf("Point should not be NULL if it is a leaf");
		return NULL;
	}
	if( (left!= NULL || right!= NULL) && data!= NULL) // TODO write log
	{
		spDestroyKDTreeNode(kdNode);
		spLoggerPrintError("Error : Point should be NULL if it isn't a leaf.", __FILE__, __func__, __LINE__);
		return NULL;
	}

	if(!left && !right && data != NULL)
	{
		kdNode -> data = data;
	}
	else if( (left!= NULL || right!= NULL) && !data ){
		kdNode -> data = NULL;
	}
	return kdNode;
}



bool isLeaf(KDTreeNode* node){
	if(!node){
		return false;
	}
	if(!node->left && !node->right){
		return true;
	}
	else{
		return false;
	}
}

int spKDTreeNodeGetDim(KDTreeNode* node){
	if(!node){
		return -1;
	}
	return node->dim ;
}


double spKDTreeNodeGetVal(KDTreeNode* node){
	if(!node){
		return 0;
	}
	return node->val ;
}

KDTreeNode* spKDTreeNodeGetLeft(KDTreeNode* node){
	if(!node){
		return NULL;
	}
	return node->left;
}

KDTreeNode* spKDTreeNodeGetRight(KDTreeNode* node){
	if(!node){
		return NULL;
	}
	return node->right ;
}

SPPoint* spKDTreeNodeGetPoint(KDTreeNode* node) {
	if(!node){
		return NULL;
	}
	return node->data;
}

int spKDTreeNodeGetPointIndex(KDTreeNode* node){
	if(!node){
		return -1;
	}
	return spPointGetIndex(node->data) ;
}

double spKDTreeNodeGetPointVal(KDTreeNode* node){
	if(!node){
		return 0;
	}
	return spPointGetAxisCoor(node->data, node->dim);
}



void spDestroyKDTreeNode(KDTreeNode* node){\
	if(!node){
		return;
	}
    if(node->data){
        spPointDestroy(node -> data);
    }
	spDestroyKDTreeNode(node->left);
	spDestroyKDTreeNode(node->right);
	free(node);
}



/**
 * *******************
 * KD TREE
 *
 * *******************
 */

KDTreeNode* spGetSPKDTreeRoot(SPKDTree* tree){
	if(!tree){
		return NULL;
	}
	return tree->root;
}


KDTreeNode* spInitSPKDTreeRec(SPKDArray kdArray, SP_KD_SPLIT_MODE splitMethod, int pointDim, int size,
		int prevSplitDim){
	if(!kdArray)
	{
		return NULL;
	}
	if(size == 1)
	{
		SPPoint* singlePoint = spGetSPKDArrayPoint(kdArray,0);
		if(!singlePoint){
			return NULL;
		}
		return spInitKDTreeNode( 0, INVALID, NULL, NULL, singlePoint);  //afaik val >=0 so INVALID is invalid. singlepoint OK?
	}
	int splitDim = -1, sizeL = 0, sizeR = 0;
	double median = 0;
	switch (splitMethod){
	case(MAX_SPREAD):;
		double* spreadArr = malloc(pointDim* sizeof(double));
		if(!spreadArr){
			return NULL;
		}

		double tmpMax = -DBL_MAX , tmpMin = DBL_MAX;
		double coor = 0;
		for (int j=0; j<pointDim; j++){
			for(int i=0; i<size; i++){

				SPPoint* point = spGetSPKDArrayPoint(kdArray,i);
				if(!point){
					free(spreadArr);
					return NULL;
				}
				coor = spPointGetAxisCoor(point,j);
				if(tmpMax <  coor) {
					tmpMax = coor;
				}

				if(tmpMin > coor){
					tmpMin = coor;
				}
				spPointDestroy(point); // needed free?

			}
			spreadArr[j] = tmpMax - tmpMin;
			tmpMax = -DBL_MAX;
			tmpMin = DBL_MAX;

		}
		for(int i=0; i<pointDim; i++){
			if(tmpMax < spreadArr[i]){
				tmpMax = spreadArr[i];
				splitDim = i;
			}
		}
		free(spreadArr);
		break;

	case( RANDOM) :;
		splitDim = rand() % pointDim;
		break;

	case( INCREMENTAL) :;
		splitDim = (prevSplitDim + 1)%pointDim;
		break;

	default:
		break;
	}
	SPKDArray* leftAndRight = spSplitSPKDArray(kdArray, splitDim);
	if(!leftAndRight){
		return NULL;
	}


	SPPoint* pointL2 = spGetSPKDArrayPoint(leftAndRight[0],size/2 - 1);
	if(!pointL2){
		spDestroyKDArray(leftAndRight[0]);
		spDestroyKDArray(leftAndRight[1]);
		free(leftAndRight);
		return NULL;
	}
	SPPoint* pointR = spGetSPKDArrayPoint(leftAndRight[1],size/2 - 1);
	if(!pointR){
		spPointDestroy(pointL2);
		spDestroyKDArray(leftAndRight[0]);
		spDestroyKDArray(leftAndRight[1]);
		free(leftAndRight);
		return NULL;
	}
	if(size%2 == 1){
		SPPoint* pointL1 = spGetSPKDArrayPoint(leftAndRight[0],size/2);
		if(!pointL1){
			spDestroyKDArray(leftAndRight[0]);
			spDestroyKDArray(leftAndRight[1]);
			spPointDestroy(pointL2);
			spPointDestroy(pointR);
			free(leftAndRight);
			return NULL;
		}
		sizeL = size/2 + 1;
		sizeR = size/2;
		median = spPointGetAxisCoor(pointL1,splitDim);
		spPointDestroy(pointL1);
	}
	else{
		sizeL = size/2;
		sizeR = size/2;
		median = (spPointGetAxisCoor(pointL2,splitDim) +  spPointGetAxisCoor(pointR,splitDim)) / 2 ;
	}

	spPointDestroy(pointL2);
	spPointDestroy(pointR);
	KDTreeNode* leftNode = spInitSPKDTreeRec(leftAndRight[0],  splitMethod, pointDim, sizeL,
			splitDim);
	if(!leftNode){
		spDestroyKDArray(leftAndRight[0]);
		spDestroyKDArray(leftAndRight[1]);
		free(leftAndRight);
		return NULL;
	}
	KDTreeNode* rightNode = spInitSPKDTreeRec(leftAndRight[1], splitMethod, pointDim, sizeR,
			splitDim);
	if(!rightNode){
		spDestroyKDTreeNode(leftNode);
		spDestroyKDArray(leftAndRight[0]);
		spDestroyKDArray(leftAndRight[1]);
		free(leftAndRight);
		return NULL;
	}
	spDestroyKDArray(leftAndRight[0]);
	spDestroyKDArray(leftAndRight[1]);
	free(leftAndRight);
	return spInitKDTreeNode(splitDim, median, leftNode, rightNode, NULL);

}


SPKDTree* spInitSPKDTree(SPPoint** arr, int size, int pointDim, SP_KD_SPLIT_MODE splitMethod){
	if(!arr || size < 1 || pointDim < 1){
		spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
		return NULL;
	}
	SPKDTree* tree = malloc(sizeof(SPKDTree));
	if(!tree){
		return NULL;
	}
	tree->features =  spInitSPKDArray(arr, size);
	if(!tree->features){
		free(tree);
		return NULL;
	}
	tree->root = spInitSPKDTreeRec(tree->features, splitMethod, pointDim, size, -1);
	if(!tree->root){
        spDestroyKDArray(tree->features);
        spDestroyKDTree(tree);
		return NULL;
	}
	return tree;
}

void spDestroyKDTree(SPKDTree* tree){
	if(!tree){
		return;
	}
	if(tree->root){
	spDestroyKDTreeNode(tree->root);
	}
	if(tree->features){
	spDestroyKDArray(tree->features);
	}
	free(tree);
}

void inOrderScanRootPoints (KDTreeNode* node){
	if(!node){
		return;
	}
	inOrderScanRootPoints(node->left);

	if( isLeaf(node)){
		// TODO print log debug
		char dbgMsg[MAX_ERR_MSG_LENGTH];
		sprintf(dbgMsg,"(%f,%f,%f) , index : %d \n", spPointGetAxisCoor(node->data,0), spPointGetAxisCoor(node->data,1) ,
				spPointGetAxisCoor(node->data,2), spPointGetIndex(node->data) );
		spLoggerPrintDebug(dbgMsg, __FILE__, __func__, __LINE__);
	}

	inOrderScanRootPoints(node->right);

}

void inOrderScanRootValues (KDTreeNode* node){
	if(!node){
		return;
	}
	inOrderScanRootValues(node->left);

	if( !isLeaf(node)){
		// TODO print log debug
		char dbgMsg[MAX_ERR_MSG_LENGTH];
		sprintf(dbgMsg,"Splitting value: %f \n", node->val);
		spLoggerPrintDebug(dbgMsg, __FILE__, __func__, __LINE__);
	}

	inOrderScanRootValues(node->right);

	return;
}
void inOrderScan(SPKDTree* tree){
	if(!tree){
		return;
	}
	// TODO print log debug
	spLoggerPrintDebug("Points and their indexes by the scan: \n", __FILE__, __func__, __LINE__);
	inOrderScanRootPoints(tree->root);
	// TODO print log debug
	spLoggerPrintDebug("Values of splitting: \n", __FILE__, __func__, __LINE__);
	inOrderScanRootValues(tree->root);
}


