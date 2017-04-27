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
#define FEATURE_DIM 128

int spPCADimension = 12;//for now only

//wont need the variables after we have system variables
SPPoint** ExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
		int spNumOfImages, sp::ImageProc spIp, int* totalNumOfFeatures){

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
	SPPoint*** Gallery = (SPPoint***) malloc ( sizeof(*Gallery) * spNumOfImages ) ;
	if(!Gallery){
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
			free(numOfFeatures);
			free(numOfFeatArray);
			free(Gallery);
			return NULL;
		}

/*		if(checker < ( strlen(directory) + strlen(imagePrefix) + strlen(imageSuffix) + sizeof(i)) ){
			printf("Failed to read all the string, read only a part!\n");
			return NULL;
		}*/
		char filePath [MAX_STRING_LENGTH];

//		checker = sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
//		if(checker < 0){
		msg = spConfigGetImagePathWithData(imgPath, directory, imagePrefix, i, featSuffix);
		if(msg != SP_CONFIG_SUCCESS){
			printf("Failed to print to string\n");
			free(numOfFeatures);
			free(numOfFeatArray);
			free(Gallery);
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
			free(numOfFeatures);
			free(numOfFeatArray);
			free(Gallery);
			return NULL;
		}
		SPPoint** imgFeatures = spIp.getImageFeatures(imgPath,i,numOfFeatures);
		if(imgFeatures == NULL){
			fclose(file);
			free(numOfFeatures);
			free(numOfFeatArray);
			free(Gallery);
			return NULL;
		}
		numOfFeatArray[i] = *numOfFeatures;
		Gallery[i] = imgFeatures;
		totalNumOfFeat += *numOfFeatures;
		checker = fprintf(file,"%d,", *numOfFeatures); //check positive
		if(checker < 0){
			printf("Failed to print to file\n");
			for(int m=0; m<i;m++){
				for(int j=0; j<numOfFeatArray[m];j++){
					spPointDestroy(Gallery[m][j]);
				}
			}
			fclose(file);
			free(numOfFeatures);
			free(numOfFeatArray);
			free(Gallery);
			return NULL;
		}
		/*
		if(checker < sizeof(*numOfFeatures) + 1 ){
			printf("Failed to print all the string, printed only a part!\n");
			return NULL;
		}*/


		for (int k=0; k<*numOfFeatures;k++){
			///* there no need for this - getImageFeatures returns NULL on an error... so we wont get here!
//			if(imgFeatures[k] == NULL){
//				for(int m=0; m<k;m++){
//					spPointDestroy(imgFeatures[m]);
//				}
//				free (imgFeatures);
//				free(numOfFeatures);
//				fclose(file);
//				return NULL;
//			}

			for( int j=0; j<spPCADimension; j++){
				checker = fprintf(file,"%f,",spPointGetAxisCoor(imgFeatures[k],j));
				if(checker < 0){
					printf("Failed to print to file\n");
					for(int m=0; m<i;m++){
						for(int j=0; j<numOfFeatArray[m];j++){
							spPointDestroy(Gallery[m][j]);
						}
					}
					fclose(file);
					free(numOfFeatures);
					free(numOfFeatArray);
					free(Gallery);

					return NULL;
				}
			}
		}
		fclose(file);
	}

	SPPoint** allImageFeatures = (SPPoint**) malloc(totalNumOfFeat * sizeof(*allImageFeatures));
	if(!allImageFeatures){
		printf("Failed to print to file\n");
		for(int m=0; m<spNumOfImages;m++){
			for(int j=0; j<numOfFeatArray[m];j++){
				spPointDestroy(Gallery[m][j]);
			}
		}
		free(numOfFeatures);
		free(numOfFeatArray);
		free(Gallery);
		return NULL;
	}
	int k = 0;
	for(int i=0; i<spNumOfImages;i++){
		for(int j = 0; j < numOfFeatArray[i]; j++){
			allImageFeatures[k] = Gallery[i][j];
			k++;
		}
	}
	*totalNumOfFeatures = totalNumOfFeat;
	free(numOfFeatures);
	free(numOfFeatArray);
	return allImageFeatures;


}


SPPoint** NonExtractionModeAct(char* directory, char* imagePrefix,
		int spNumOfImages, int* totalNumOfFeatures){
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
		return NULL;
	}
	int totalNumOfFeat = 0;
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
		checker = sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
		if(checker < 0 ){
			printf("failed reading to string\n");
			return NULL;
		}
		FILE* file = fopen(filePath, &readMode);
		if(file == NULL)
		{
			free(gallery);
			free(featValArray);
			free(numOfFeatures);
			return NULL;
		}
		checker = fscanf(file, "%d,",numOfFeatures);
		if(checker < 0){
			printf("failed scanning from file \n");
			return NULL;
		}
		numOfFeatArray[i] = *numOfFeatures;
		totalNumOfFeat += *numOfFeatures;
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
				checker = fscanf(file, "%lf,",&featValArray[j]); // check if something fucks up, other fscanf too
				if(checker < 0){
					printf("failed scanning from file \n");
					return NULL;
				}
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

	SPPoint** allImageFeatures = (SPPoint**) malloc (totalNumOfFeat * sizeof(SPPoint*) );
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
	free(gallery); //should be ok
	return allImageFeatures;
}
