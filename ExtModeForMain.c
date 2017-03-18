/*
 * ExtModeForMain.c
 *
 *  Created on: Mar 14, 2017
 *      Author: user
 */

//maybe static char*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SPImageProc.h"

#define MAX_STRING_LENGTH 1024
#define FILE_PATH_PATTERN "%s%s%d%s"
#define FEATURE_DIM 128

int spPCADimension = 12;//for now only

//wont need the variables after we have system variables
void ExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
		int spNumOfImages, int spNumOfFeatures){
	char featSuffix[7] = ".feats";
	int* numOfFeatures = malloc(sizeof(int));
	if(numOfFeatures == NULL){
		return;
	}

	for(int i=0; i<spNumOfImages;i++){
		//getting the paths
		char imgPath[MAX_STRING_LENGTH];
		sprintf(imgPath, FILE_PATH_PATTERN, directory, imagePrefix, i, imageSuffix); //check positive
		char filePath[MAX_STRING_LENGTH];
		sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
		FILE* file = fopen(filePath, 'w');
		if(file == NULL)
		{
			free(numOfFeatures);
			return;
		}
		SPPoint** imgFeatures = getImageFeatures(imgPath,i,numOfFeatures); //possible leak
		if(imgFeatures == NULL){
			free(numOfFeatures);
			fclose(file);
			return;
		}

		fprintf(file,"%d,", *numOfFeatures); //check positive
		for (int k=0; k<*numOfFeatures;k++){
			if(imgFeatures[k] == NULL){
				for(int m=0; m<k;m++){
					spPointDestroy(imgFeatures[m]);
				}
				free (imgFeatures);
				free(numOfFeatures);
				fclose(file);
				return;
			}
			for( int j=0; j<spPCADimension; j++){
				fprintf(file,"%f,",spPointGetAxisCoor(imgFeatures[k],j));
			}
			//fprintf(file," ");
		}
		fclose(file);
		for (int k=0; k<*numOfFeatures;k++){
			spPointDestroy(imgFeatures[k]);
		}
	}


}


SPPoint*** NotExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
		int spNumOfImages, int spNumOfFeatures){

	//creating the returned variable
	SPPoint*** gallery = malloc(sizeof(SPPoint**) * spNumOfImages);
	if(gallery == NULL){
		return NULL;
	}
	//null check
	char featSuffix[7] = ".feats";
	int* numOfFeatures = malloc(sizeof(int));
	if(numOfFeatures == NULL){
		free(gallery);
		return NULL;
	}
	double* featValArray = malloc(sizeof(double)*spPCADimension);
	if(featValArray == NULL){
		free(gallery);
		free(numOfFeatures);
		return NULL;
	}
	for(int i=0; i<spNumOfImages; i++){
		char filePath [MAX_STRING_LENGTH];
		sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
		FILE* file = fopen(filePath, 'r');
		if(file == NULL)
		{
			free(gallery);
			free(featValArray);
			free(numOfFeatures);
			return NULL;
		}
		fscanf(file, "%d,",*numOfFeatures);
		gallery[i] = malloc(sizeof(SPPoint*) * *numOfFeatures);
		if(gallery[i] == NULL){
			for(int b=0; b<i; b++){
				for( int j=0; j<spPCADimension; j++){
					spPointDestroy(gallery[b][j]);
				}
				free(gallery[b]);
			}
			free(gallery);
			free(featValArray);
			free(numOfFeatures);
			fclose(file);
			return NULL;
		}
		for(int k = 0; k<*numOfFeatures;k++){
			for(int j=0; j<spPCADimension; j++){
				fscanf(file, "%f,",featValArray[j]);
			}
			gallery[i][k]= spPointCreate(featValArray,spPCADimension,i); //index of feature is image's index
			if(gallery[i][k] == NULL){
				for(int b=0; b<i; b++){
					for( int j=0; j<spPCADimension; j++){
						spPointDestroy(gallery[b][j]);
					}
					free(gallery[b]);
				}
				for(int j=0; j<k;j++){
					spPointDestroy(gallery[i][k]);
				}
				free(gallery[i]);
				free(gallery);
				free(featValArray);
				free(numOfFeatures);
				fclose(file);
				return NULL;
			}
		}
		fclose(file);
	}
	free(featValArray);
	free(numOfFeatures);
	return gallery;
}

