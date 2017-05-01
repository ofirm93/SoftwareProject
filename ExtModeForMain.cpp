///*
// * ExtModeForMain.cpp
// *
// *  Created on: Mar 14, 2017
// *      Author: user
// */
//
//
////maybe static char*
//#include "ExtModeForMain.h"
//
//#define MAX_STRING_LENGTH 1024
//
//#define MAX_ERR_MSG_LENGTH 1024
//
//#define ERR_MSG_INVALID_ARG "Error : One of the arguments supplied to the method is invalid."
//#define ERR_MSG_INVALID_ARG_IN_METHOD "Error : One of the arguments supplied to the method %s is invalid."
//#define ERR_MSG_CANNOT_ALLOCATE_MEM "Error : Couldn't allocate needed memory."
//#define ERR_MSG_GET_IMG_PATH "spConfigGetImagePath()"
//#define ERR_MSG_INDEX_OUT_OF_RANGE "Error : The index %d of a best candidate is out of range"
//#define ERR_MSG_GENERAL "General error. Unexpectedly, the program reached to a code where it shouldn't supposed to."
//#define ERR_MSG_MORE_IMG_THAN_AVAILABLE "Error : asked for %d closest images while there are only %d images at all."
//#define ERR_MSG_CANNOT_EXT_FEAT "Error : Couldn't extract feature from an image.\nPlease check if Path : %s is correct."
//#define ERR_MSG_CANNOT_FIND_CLOSE_FEAT "Error : Couldn't find the closest features to a feature."
//#define ERR_MSG_BPQUEUE_PEEK "spBPQueuePeek()"
//#define ERR_MSG_BPQUEUE_DEQUEUE "spBPQueueDequeue()"
//#define ERR_MSG_CANNOT_PEEK_EMPTY "Error : Couldn't peek to the queue top because it's empty."
//#define ERR_MSG_CANNOT_DEQUEUE_EMPTY "Error : Couldn't dequeue element from the queue because it's empty."
//#define ERR_MSG_MTHD_GET_IMG_PATH_DATA "spConfigGetImagePathWithData()"
//#define ERR_MSG_FAIL_PRINT_TO_FILE "Error : Failed to print to file."
//#define ERR_MSG_FAIL_SCAN_FILE "Error : Failed reading from features file.\nPlease check if Path : %s is correct."
//
//void spDestroySPPoint2DimArray(SPPoint*** gallery, int* numOfFeatArray, int sizeOfGallery){
//	if(!gallery|| !numOfFeatArray){
//		return;
//	}
//	for(int i = 0; i< sizeOfGallery; i++){
//		spDestroySPPointArray(gallery[i],numOfFeatArray[i]);
//	}
//	free(gallery);
//}
//
////wont need the variables after we have system variables
//SPPoint** ExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
//		int spNumOfImages, sp::ImageProc spIp, int* totalNumOfFeatures, int spPCADimension){
//	if(!directory || !imagePrefix || !imageSuffix || !totalNumOfFeatures){
//        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
//        return NULL;
//	}
//	char featSuffix[7] = ".feats";
//	int* numOfFeatures = (int*) malloc(sizeof(int));
//	if(!numOfFeatures){
//		spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
//		return NULL;
//	}
//	int* numOfFeatArray = (int*) malloc(sizeof(int) * spNumOfImages);
//	if(!numOfFeatArray){
//		spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
//		free(numOfFeatures);
//		return NULL;
//	}
//	int totalNumOfFeat = 0;
//	int checker = 0;
//	SPPoint*** gallery = (SPPoint***) malloc ( sizeof(*gallery) * spNumOfImages ) ;
//	if(!gallery){
//		free(numOfFeatures);
//		free(numOfFeatArray);
//		spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
//		return NULL;
//	}
//	for(int i=0; i<spNumOfImages;i++){
//		//getting the paths
//		char imgPath[MAX_STRING_LENGTH];
////		checker = sprintf(imgPath, FILE_PATH_PATTERN, directory, imagePrefix, i, imageSuffix); //check positive
//		SP_CONFIG_MSG msg = spConfigGetImagePathWithData(imgPath, directory, imagePrefix, i, imageSuffix);
//		if(msg != SP_CONFIG_SUCCESS){
//
//			char errorMsg[MAX_ERR_MSG_LENGTH];
//			sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_MTHD_GET_IMG_PATH_DATA);
//			spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
//  	  spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			return NULL;
//		}
//
///*		if(checker < ( strlen(directory) + strlen(imagePrefix) + strlen(imageSuffix) + sizeof(i)) ){
//			printf("Failed to read all the string, read only a part!\n");
//			return NULL;
//		}*/
//		char filePath [MAX_STRING_LENGTH];
//
////		checker = sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
////		if(checker < 0){
//		msg = spConfigGetImagePathWithData(filePath, directory, imagePrefix, i, featSuffix);
//		if(msg != SP_CONFIG_SUCCESS){
//
//			char errorMsg[MAX_ERR_MSG_LENGTH];
//			sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_MTHD_GET_IMG_PATH_DATA);
//			spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
//  		spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			return NULL;
//		}
//		/*
//		if(checker < ( strlen(directory) + strlen(imagePrefix) + strlen(featSuffix) + sizeof(i)) ){
//			printf("Failed to read all the string, read only a part!\n");
//			return NULL;
//		}*/
//		const char * destPtr = filePath;
//		const char writeMode = 'w';
//		FILE* file = fopen(destPtr, &writeMode);
//		if(file == NULL)
//		{
//			char errorMsg[MAX_ERR_MSG_LENGTH];
//			sprintf(errorMsg, "Error : Couldn't open a file.\nPlease check if Path : %s is correct.", destPtr);
//			spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
//			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			return NULL;
//		}
//		SPPoint** imgFeatures = spIp.getImageFeatures(imgPath,i,numOfFeatures);
//		if(imgFeatures == NULL){
//			char errorMsg[MAX_ERR_MSG_LENGTH];
//			sprintf(errorMsg, ERR_MSG_CANNOT_EXT_FEAT, imgPath);
//			spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
//  		spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			fclose(file);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			return NULL;
//		}
//		for(int k = 0 ; k<*numOfFeatures; k++){
//			if(!imgFeatures[k]){
//				spDestroySPPointArray(imgFeatures,k);
//				fclose(file);
//				free(numOfFeatures);
//				free(numOfFeatArray);
//				free(gallery);
//				return NULL;
//			}
//		}
//		numOfFeatArray[i] = *numOfFeatures;
//		gallery[i] = imgFeatures;
//		totalNumOfFeat += *numOfFeatures;
//		checker = fprintf(file,"%d,", *numOfFeatures); //check positive
//		if(checker < 0){
//			spLoggerPrintError(ERR_MSG_FAIL_PRINT_TO_FILE, __FILE__, __func__, __LINE__);
//			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			fclose(file);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			return NULL;
//		}
//		/*
//		if(checker < sizeof(*numOfFeatures) + 1 ){
//			printf("Failed to print all the string, printed only a part!\n");
//			return NULL;
//		}*/
//
//
//		for (int k=0; k<*numOfFeatures;k++){
//			for( int j=0; j<spPCADimension; j++){
//				checker = fprintf(file,"%f,",spPointGetAxisCoor(imgFeatures[k],j));
//				if(checker < 0){
//					spLoggerPrintError(ERR_MSG_FAIL_PRINT_TO_FILE, __FILE__, __func__, __LINE__);
//					spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//					fclose(file);
//					free(numOfFeatures);
//					free(numOfFeatArray);
//					return NULL;
//				}
//			}
//		}
//		fclose(file);
//	}
//
//	SPPoint** allImageFeatures = (SPPoint**) malloc(totalNumOfFeat * sizeof(*allImageFeatures));
//	if(!allImageFeatures){
//		spLoggerPrintError(ERR_MSG_FAIL_PRINT_TO_FILE, __FILE__, __func__, __LINE__);
//		spDestroySPPoint2DimArray(gallery,numOfFeatArray,spNumOfImages);
//		free(numOfFeatures);
//		free(numOfFeatArray);
//		return NULL;
//	}
//	int k = 0;
//	for(int i=0; i<spNumOfImages;i++){
//		for(int j = 0; j < numOfFeatArray[i]; j++){
//			allImageFeatures[k] = gallery[i][j];
//			k++;
//		}
//	}
//	*totalNumOfFeatures = totalNumOfFeat;
//	free(numOfFeatures);
//	free(numOfFeatArray);
//	free(gallery);// TODO NOT destroy - we need the points!
//	return allImageFeatures;
//}
//
//
//SPPoint** NonExtractionModeAct(char* directory, char* imagePrefix,
//		int spNumOfImages, int* totalNumOfFeatures,  int spPCADimension){
//	if(!directory || !imagePrefix || !totalNumOfFeatures){
//		return NULL;
//	}
//	const char readMode = 'r';
//	int checker = 0;
//	//creating the returned variable
//	SPPoint*** gallery = (SPPoint***) malloc(sizeof(SPPoint**) * spNumOfImages);
//	if(gallery == NULL){
//		return NULL;
//	}
//	int* numOfFeatArray = (int*) malloc(sizeof(int) * spNumOfImages);
//	if(!numOfFeatArray){
//		spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
//		free(gallery); // TODO bar? : why didn't use 		spDestroySPPoint2DimArray(gallery,numOfFeatArray,spNumOfImages);
//		return NULL;
//	}
//	int totalNumOfFeat = 0;
//	//null check
//	char featSuffix[7] = ".feats";
//	int* numOfFeatures = (int*) malloc(sizeof(int));
//	if(numOfFeatures == NULL){
//		free(numOfFeatArray);
//		free(gallery);
//		return NULL;
//	}
//	double* featValArray = (double*) malloc(sizeof(double)*spPCADimension);
//	if(featValArray == NULL){
//		free(gallery);
//		free(numOfFeatures);
//		free(numOfFeatArray);
//		return NULL;
//	}
//	for(int i=0; i<spNumOfImages; i++){
//		char filePath [MAX_STRING_LENGTH];
//		/**
//		checker = sprintf(filePath, FILE_PATH_PATTERN, directory, imagePrefix, i, featSuffix); //check positive
//		if(checker < 0 ){
//			char errorMsg[MAX_ERR_MSG_LENGTH];
//			sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_MTHD_GET_IMG_PATH_DATA);
//			spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
//			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			free(featValArray);
//			return NULL;
//		}
//		*/
//
//		SP_CONFIG_MSG msg = spConfigGetImagePathWithData(filePath, directory, imagePrefix, i, featSuffix);
//		if(msg != SP_CONFIG_SUCCESS){
//			char errorMsg[MAX_ERR_MSG_LENGTH];
//			sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_MTHD_GET_IMG_PATH_DATA);
//			spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			free(featValArray);
//			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			return NULL;
//		}
//		FILE* file = fopen(filePath, &readMode);
//		if(file == NULL)
//		{
//			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			free(featValArray);
//			return NULL;
//		}
//		checker = fscanf(file, "%d,",numOfFeatures);
//		if(checker < 0){
//			char errorMsg[MAX_ERR_MSG_LENGTH];
//			sprintf(errorMsg, ERR_MSG_FAIL_SCAN_FILE, filePath);
//			spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
//			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			free(featValArray);
//			fclose(file);
//			return NULL;
//		}
//		numOfFeatArray[i] = *numOfFeatures;
//		totalNumOfFeat += *numOfFeatures;
//		gallery[i] = (SPPoint**) malloc(sizeof(SPPoint*) * *numOfFeatures);
//		if(gallery[i] == NULL){
//			spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//			free(numOfFeatures);
//			free(numOfFeatArray);
//			free(featValArray);
//			fclose(file);
//			return NULL;
//		}
//		for(int k = 0; k<*numOfFeatures;k++){
//			for(int j=0; j<spPCADimension; j++){
//				checker = fscanf(file, "%lf,",&featValArray[j]); // check if something fucks up, other fscanf too
//				if(checker < 0){
//					char errorMsg[MAX_ERR_MSG_LENGTH];
//					sprintf(errorMsg, ERR_MSG_FAIL_SCAN_FILE, filePath);
//					spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
//					spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//					free(numOfFeatures);
//					free(numOfFeatArray);
//					free(featValArray);
//					return NULL;
//				}
//			}
//			gallery[i][k]= spPointCreate(featValArray,spPCADimension,i); //index of feature is image's index
//			if(gallery[i][k] == NULL){
//				spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
//				free(numOfFeatures);
//				free(numOfFeatArray);
//				free(featValArray);
//				fclose(file);
//				return NULL;
//			}
//		}
//		fclose(file);
//	}
//
//	SPPoint** allImageFeatures = (SPPoint**) malloc (totalNumOfFeat * sizeof(SPPoint*) );
//	if(!allImageFeatures){
//		spDestroySPPoint2DimArray(gallery,numOfFeatArray,spNumOfImages);
//		free(numOfFeatures);
//		free(numOfFeatArray);
//		free(featValArray);
//        return NULL;
//	}
//	int k = 0;
//	for(int i=0; i<spNumOfImages; i++){
//		for(int j=0; j<numOfFeatArray[i]; j++){
//			allImageFeatures[k] = gallery[i][j];
//			k++;
//		}
//	}
//
//	*totalNumOfFeatures = totalNumOfFeat;
//	free(featValArray);
//	free(numOfFeatures);
//	free(numOfFeatArray);
//	for(int l=0; l<spNumOfImages; l++){
//		free(gallery[l]);
//	}
//	free(gallery); //NOT destroy - we need the points.
//	return allImageFeatures;
//}
