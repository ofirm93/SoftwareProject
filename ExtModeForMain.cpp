/*
 * ExtModeForMain.c
 *
 *  Created on: Mar 14, 2017
 *      Author: user
 */


//maybe static char*
#include "ExtModeForMain.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define MAX_STRING_LENGTH 1024
#define FILE_PATH_PATTERN "%s%s%d%s"
#define FEATURE_DIM 128

int spPCADimension = 12;//for now only

//wont need the variables after we have system variables
void ExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
		int spNumOfImages, int spNumOfFeatures, sp::ImageProc spIp){

	char featSuffix[7] = ".feats";
	int* numOfFeatures = (int*) malloc(sizeof(int));
	if(numOfFeatures == NULL){
		return;
	}

	for(int i=0; i<spNumOfImages;i++){
		//getting the paths
		char imgPath[MAX_STRING_LENGTH];
		sprintf(imgPath, FILE_PATH_PATTERN, directory, imagePrefix, i, imageSuffix); //check positive
		char* filePath = (char*)malloc(MAX_STRING_LENGTH*sizeof(char));
		sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
		const char * destPtr = filePath;
		const char writeMode = 'w';
		FILE* file = fopen(destPtr, &writeMode);
		if(file == NULL)
		{
			free(numOfFeatures);
			return;
		}
		SPPoint** imgFeatures = spIp.getImageFeatures(imgPath,i,numOfFeatures); //possible leak
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
	const char readMode = 'r';
	//creating the returned variable
	SPPoint*** gallery = (SPPoint***) malloc(sizeof(SPPoint**) * spNumOfImages);
	if(gallery == NULL){
		return NULL;
	}
	//null check
	char featSuffix[7] = ".feats";
	int* numOfFeatures = (int*) malloc(sizeof(int));
	if(numOfFeatures == NULL){
		free(gallery);
		return NULL;
	}
	double* featValArray = (double*) malloc(sizeof(double)*spPCADimension);
	if(featValArray == NULL){
		free(gallery);
		free(numOfFeatures);
		return NULL;
	}
	for(int i=0; i<spNumOfImages; i++){
		char filePath [MAX_STRING_LENGTH];
		sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
		FILE* file = fopen(filePath, &readMode);
		if(file == NULL)
		{
			free(gallery);
			free(featValArray);
			free(numOfFeatures);
			return NULL;
		}
		fscanf(file, "%d,",numOfFeatures);
		gallery[i] = (SPPoint**) malloc(sizeof(SPPoint*) * *numOfFeatures);
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
				fscanf(file, "%lf,",&featValArray[j]); // check if something fucks up, other fscanf too
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


