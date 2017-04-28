/*
 * ExtModeForMain.cpp
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


void spDestroySPPoint2DimArray(SPPoint*** gallery, int* numOfFeatArray, int sizeOfGallery){
	if(!gallery|| !numOfFeatArray){
		return;
	}
	for(int i = 0; i< sizeOfGallery; i++){
		spDestroySPPointArray(gallery[i],numOfFeatArray[i]);
	}
	free(gallery);
}

//wont need the variables after we have system variables
SPPoint** ExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
		int spNumOfImages, sp::ImageProc spIp, int* totalNumOfFeatures, int spPCADimension){
	if(!directory || !imagePrefix || !imageSuffix || !totalNumOfFeatures){
		return NULL;
	}
	char featSuffix[7] = ".feats";
	int* numOfFeatures = (int*) malloc(sizeof(int));
	if(!numOfFeatures){
		printf("failed to allocate memory\n");
		return NULL;
	}
	int* numOfFeatArray = (int*) malloc(sizeof(int) * spNumOfImages);
	if(!numOfFeatArray){
		printf("failed to allocate memory\n");
		free(numOfFeatures);
		return NULL;
	}
	int totalNumOfFeat = 0;
	int checker = 0;
	SPPoint*** gallery = (SPPoint***) malloc ( sizeof(*gallery) * spNumOfImages ) ;
	if(!gallery){
		free(numOfFeatures);
		free(numOfFeatArray);
		printf("failed to allocate memory\n");
		return NULL;
	}
	for(int i=0; i<spNumOfImages;i++){
		//getting the paths
		char imgPath[MAX_STRING_LENGTH];
//		checker = sprintf(imgPath, FILE_PATH_PATTERN, directory, imagePrefix, i, imageSuffix); //check positive
		SP_CONFIG_MSG msg = spConfigGetImagePathWithData(imgPath, directory, imagePrefix, i, imageSuffix);
		if(msg != SP_CONFIG_SUCCESS){
			printf("Failed to print to string\n");
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			free(numOfFeatures);
			free(numOfFeatArray);
			return NULL;
		}

/*		if(checker < ( strlen(directory) + strlen(imagePrefix) + strlen(imageSuffix) + sizeof(i)) ){
			printf("Failed to read all the string, read only a part!\n");
			return NULL;
		}*/
		char filePath [MAX_STRING_LENGTH];

//		checker = sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
//		if(checker < 0){
		msg = spConfigGetImagePathWithData(filePath, directory, imagePrefix, i, featSuffix);
		if(msg != SP_CONFIG_SUCCESS){
			printf("Failed to print to string\n");
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			free(numOfFeatures);
			free(numOfFeatArray);
			return NULL;
		}
		/*
		if(checker < ( strlen(directory) + strlen(imagePrefix) + strlen(featSuffix) + sizeof(i)) ){
			printf("Failed to read all the string, read only a part!\n");
			return NULL;
		}*/
		const char * destPtr = filePath;
		const char writeMode = 'w';
		FILE* file = fopen(destPtr, &writeMode);
		if(file == NULL)
		{
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			free(numOfFeatures);
			free(numOfFeatArray);
			return NULL;
		}
		SPPoint** imgFeatures = spIp.getImageFeatures(imgPath,i,numOfFeatures);
		if(imgFeatures == NULL){
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			fclose(file);
			free(numOfFeatures);
			free(numOfFeatArray);
			return NULL;
		}
		for(int k = 0 ; k<*numOfFeatures; k++){
			if(!imgFeatures[k]){
				spDestroySPPointArray(imgFeatures,k);
				fclose(file);
				free(numOfFeatures);
				free(numOfFeatArray);
				free(gallery);
				return NULL;
			}
		}
		numOfFeatArray[i] = *numOfFeatures;
		gallery[i] = imgFeatures;
		totalNumOfFeat += *numOfFeatures;
		checker = fprintf(file,"%d,", *numOfFeatures); //check positive
		if(checker < 0){
			printf("Failed to print to file\n");
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			fclose(file);
			free(numOfFeatures);
			free(numOfFeatArray);
			return NULL;
		}
		/*
		if(checker < sizeof(*numOfFeatures) + 1 ){
			printf("Failed to print all the string, printed only a part!\n");
			return NULL;
		}*/


		for (int k=0; k<*numOfFeatures;k++){
			for( int j=0; j<spPCADimension; j++){
				checker = fprintf(file,"%f,",spPointGetAxisCoor(imgFeatures[k],j));
				if(checker < 0){
					printf("Failed to print to file\n");
					spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
					fclose(file);
					free(numOfFeatures);
					free(numOfFeatArray);
					return NULL;
				}
			}
		}
		fclose(file);
	}

	SPPoint** allImageFeatures = (SPPoint**) malloc(totalNumOfFeat * sizeof(*allImageFeatures));
	if(!allImageFeatures){
		printf("Failed to print to file\n");
		spDestroySPPoint2DimArray(gallery,numOfFeatArray,spNumOfImages);
		free(numOfFeatures);
		free(numOfFeatArray);
		return NULL;
	}
	int k = 0;
	for(int i=0; i<spNumOfImages;i++){
		for(int j = 0; j < numOfFeatArray[i]; j++){
			allImageFeatures[k] = gallery[i][j];
			k++;
		}
	}
	*totalNumOfFeatures = totalNumOfFeat;
	free(numOfFeatures);
	free(numOfFeatArray);
	free(gallery);// NOT destroy - we need the points!
	return allImageFeatures;
}


SPPoint** NonExtractionModeAct(char* directory, char* imagePrefix,
		int spNumOfImages, int* totalNumOfFeatures,  int spPCADimension){
	if(!directory || !imagePrefix || !totalNumOfFeatures){
		return NULL;
	}
	const char readMode = 'r';
	int checker = 0;
	//creating the returned variable
	SPPoint*** gallery = (SPPoint***) malloc(sizeof(SPPoint**) * spNumOfImages);
	if(gallery == NULL){
		return NULL;
	}
	int* numOfFeatArray = (int*) malloc(sizeof(int) * spNumOfImages);
	if(!numOfFeatArray){
		printf("failed to allocate memory\n");
		free(gallery);
		return NULL;
	}
	int totalNumOfFeat = 0;
	//null check
	char featSuffix[7] = ".feats";
	int* numOfFeatures = (int*) malloc(sizeof(int));
	if(numOfFeatures == NULL){
		free(numOfFeatArray);
		free(gallery);
		return NULL;
	}
	double* featValArray = (double*) malloc(sizeof(double)*spPCADimension);
	if(featValArray == NULL){
		free(gallery);
		free(numOfFeatures);
		free(numOfFeatArray);
		return NULL;
	}
	for(int i=0; i<spNumOfImages; i++){
		char filePath [MAX_STRING_LENGTH];
		checker = sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
		if(checker < 0 ){
			printf("failed reading to string\n");
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			free(numOfFeatures);
			free(numOfFeatArray);
			free(featValArray);
			return NULL;
		}
		FILE* file = fopen(filePath, &readMode);
		if(file == NULL)
		{
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			free(numOfFeatures);
			free(numOfFeatArray);
			free(featValArray);
			return NULL;
		}
		checker = fscanf(file, "%d,",numOfFeatures);
		if(checker < 0){
			printf("failed scanning from file \n");
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			free(numOfFeatures);
			free(numOfFeatArray);
			free(featValArray);
			return NULL;
		}
		numOfFeatArray[i] = *numOfFeatures;
		totalNumOfFeat += *numOfFeatures;
		gallery[i] = (SPPoint**) malloc(sizeof(SPPoint*) * *numOfFeatures);
		if(gallery[i] == NULL){
			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
			free(numOfFeatures);
			free(numOfFeatArray);
			free(featValArray);
			fclose(file);
			return NULL;
		}
		for(int k = 0; k<*numOfFeatures;k++){
			for(int j=0; j<spPCADimension; j++){
				checker = fscanf(file, "%lf,",&featValArray[j]); // check if something fucks up, other fscanf too
				if(checker < 0){
					printf("failed scanning from file \n");
					spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
					free(numOfFeatures);
					free(numOfFeatArray);
					free(featValArray);
					return NULL;
				}
			}
			gallery[i][k]= spPointCreate(featValArray,spPCADimension,i); //index of feature is image's index
			if(gallery[i][k] == NULL){
				spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
				free(numOfFeatures);
				free(numOfFeatArray);
				free(featValArray);
				fclose(file);
				return NULL;
			}
		}
		fclose(file);
	}

	SPPoint** allImageFeatures = (SPPoint**) malloc (totalNumOfFeat * sizeof(SPPoint*) );
	if(!allImageFeatures){
		spDestroySPPoint2DimArray(gallery,numOfFeatArray,spNumOfImages);
		free(numOfFeatures);
		free(numOfFeatArray);
		free(featValArray);
	}
	int k = 0;
	for(int i=0; i<spNumOfImages; i++){
		for(int j=0; j<numOfFeatArray[j]; j++){
			allImageFeatures[k] = gallery[i][j];
			k++;
		}
	}

	*totalNumOfFeatures = totalNumOfFeat;
	free(featValArray);
	free(numOfFeatures);
	free(numOfFeatArray);
	free(gallery); //NOT destroy - we need the points.
	return allImageFeatures;
}
