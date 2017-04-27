#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SPConfig.h"

#define READ_MODE "r"
#define MAX_LINE_LENGTH 1024

struct sp_config_t{
	char* imagesDirectory;
	char* imagesPrefix;
	char* imagesSuffix;
	int numOfImages;
	int PCADimension;
	char* PCAFilename;
	int numOfFeatures;
	bool extractionMode;
	int numOfSimilarImages;
	SP_KD_SPLIT_MODE KDTreeSplitMethod;
	int KNN;
	bool minimalGUI;
	int loggerLevel;
	char* loggerFilename;
};

SPConfig spDefaultConfigConstructor(){
    SPConfig config = malloc(sizeof(struct sp_config_t));
	if(!config){
		return NULL;
	}
	config->PCADimension = 20;
	config->PCAFilename = "pca.yml";
	config->numOfFeatures = 100;
	config->extractionMode = true;
	config->numOfSimilarImages = 1;
	config->KDTreeSplitMethod = MAX_SPREAD;
	config->KNN = 1;
	config->minimalGUI = false;
	config->loggerLevel = 3;
	config->loggerFilename = NULL;
	return config;
}


// TODO Delete the next method - its for testing only
SPConfig spConfigConstructor(char *imagesDirectory,
                       char *imagesPrefix,
                       char *imagesSuffix,
                       int numOfImages,
                       int PCADimension,
                       char *PCAFilename,
                       int numOfFeatures,
                       bool extractionMode,
                       int numOfSimilarImages,
                       SP_KD_SPLIT_MODE KDTreeSplitMethod,
                       int KNN,
                       bool minimalGUI,
                       int loggerLevel,
                       char *loggerFilename) {
    SPConfig config = spDefaultConfigConstructor();
    if(!config){
        printf("[Ofir_Test] Allocation error in spBuildConfig()");
        return NULL;
    }
    config->imagesDirectory = imagesDirectory;
    config->imagesPrefix = imagesPrefix;
    config->imagesSuffix = imagesSuffix;
    config->numOfImages = numOfImages;
    config->PCADimension = PCADimension;
    config->PCAFilename = PCAFilename;
    config->numOfFeatures = numOfFeatures;
    config->extractionMode = extractionMode;
    config->numOfSimilarImages = numOfSimilarImages;
    config->KDTreeSplitMethod = KDTreeSplitMethod;
    config->KNN = KNN;
    config->minimalGUI = minimalGUI;
    config->loggerLevel = loggerLevel;
    config->loggerFilename = loggerFilename;
    return config;
}

// TODO Delete the next method - its for testing only
bool spIsStringsEqual(char *paramName, char *str, char *strExp){
    if(strcmp(str,strExp) != 0){
        printf("[Ofir_Test] The parameter %s in the created config is %s while the expected is %s", paramName,str, strExp);
        return false;
    }
    return true;
}

// TODO Delete the next method - its for testing only
bool spIsNumbersEqual(char *paramName, int num, int numExp){
    if(num != numExp){
        printf("[Ofir_Test] The parameter %s in the created config is %d while the expected is %d", paramName,num, numExp);
        return false;
    }
    return true;
}

// TODO Delete the next method - its for testing only
bool spIsConfigEqual(SPConfig config, SPConfig configExp) {
    if(!config || !configExp){
        printf("[Ofir_Test] Null pointer error in  spIsConfigEqual()");
        return false;
    }
    if((!spIsStringsEqual("imagesDirectory", config->imagesDirectory, configExp->imagesDirectory)) ||
       (!spIsStringsEqual("imagesPrefix", config->imagesPrefix, configExp->imagesPrefix)) ||
       (!spIsStringsEqual("imagesSuffix", config->imagesSuffix, configExp->imagesSuffix)) ||
       (!spIsNumbersEqual("numOfImages", config->numOfImages, configExp->numOfImages)) ||
       (!spIsNumbersEqual("PCADimension", config->PCADimension, configExp->PCADimension)) ||
       (!spIsStringsEqual("PCAFilename", config->PCAFilename, configExp->PCAFilename)) ||
       (!spIsNumbersEqual("numOfFeatures", config->numOfFeatures, configExp->numOfFeatures)) ||
       (!spIsNumbersEqual("extractionMode", config->extractionMode, configExp->extractionMode)) ||
       (!spIsNumbersEqual("numOfSimilarImages", config->numOfSimilarImages, configExp->numOfSimilarImages)) ||
       (!spIsNumbersEqual("KDTreeSplitMethod", config->KDTreeSplitMethod, configExp->KDTreeSplitMethod)) ||
       (!spIsNumbersEqual("KNN", config->KNN, configExp->KNN)) ||
       (!spIsNumbersEqual("minimalGUI", config->minimalGUI, configExp->minimalGUI)) ||
       (!spIsNumbersEqual("loggerLevel", config->loggerLevel, configExp->loggerLevel)) ||
       (!spIsStringsEqual("loggerFilename", config->loggerFilename, configExp->loggerFilename))
            ){
        return false;
    }
    return true;
}
void spPrintInvalidLineError(const char *filename, int lineNum) {
    printf("\"File: %s\n",filename);
    printf("Line: %d\n",lineNum);
    printf("Message: Invalid configuration line\"\n");
}

void spPrintInvalidValueError(const char *filename, int lineNum) {
    printf("\"File: %s\n",filename);
    printf("Line: %d\n",lineNum);
    printf("Message: Invalid value - constraint not met\"\n");
}
void spPrintUndefinedValueError(const char *filename, int lineNum, const char *paramName) {
    printf("\"File: %s\n",filename);
    printf("Line: %d\n",lineNum);
    printf("Message: Parameter %s is not set\"\n", paramName);
}

/**
 * assert str doesn't start with ' ' (space).
 * This method checks whether str represent one word (no spaces in the middle)
 * If so it returns true and removes any extra spaces from the string.
 * Else it returns false
 *
 * @param str the string to check
 * @assert strlen(str) > 0 && str[0] != ' '
 * @return  true if str is one word, otherwise false
 */
bool spTurnIntoWord(char* str){
    size_t length = strlen(str);
    size_t n = 0;
    size_t i = 0;
    while (i < length && str[i] != ' ' && str[i] != '\n' && str[i] != '\r'){	// determining the first word's numOfPoints
        n++;
        i++;
    }
    if (i >= length){	// if string ended than it's correct and we're done
		char temp[MAX_LINE_LENGTH];
		strcpy(temp, str);
		snprintf(str, n + 1, "%s", temp); // copy the first word

//        strncpy(str, str, n);	// copy the first word TODO delete if works
        return true;
    }
    while (i < length && (str[i] == ' ' || str[i] == '\n' || str[i] == '\r')){	// pass any spaces
        i++;
    }
    if (i >= length){	// if string ended than it's correct and we're done
        char temp[MAX_LINE_LENGTH];
		strcpy(temp, str);
		snprintf(str, n + 1, "%s", temp); // copy the first word
//        strncpy(str, str, n);	// copy the first word TODO delete if works
        return true;
    }
    return false;	// if string didn't end than it's incorrect
}

bool spIsNonNegativeInteger(char *str){
    if(!str){
        return false;
    }
    for(unsigned int i = 0; i < strlen(str); i++){
        if(!isdigit(str[i])){
            return false;
        }
    }
    return true;
}

/**
 * @assert strlen(value) > 0 && value[0] != ' '
 */
bool spHandleStringProperty(char** property, bool* propertyCheck, char* value, SP_CONFIG_MSG* msg,
							const char *filename, int lineNum){
	if(!property || !msg || !filename){
		return false;
	}
	if(!spTurnIntoWord(value)){
		spPrintInvalidValueError(filename, lineNum);
		*msg = SP_CONFIG_INVALID_STRING;
		return false;
	}
	*property = malloc((strlen(value) + 1) * sizeof(char));
	if(!(*property)){
		*msg = SP_CONFIG_ALLOC_FAIL;
		return false;
	}
	snprintf(*property, strlen(value) + 1, "%s", value);
	if(propertyCheck){
		*propertyCheck = false;
	}
	return true;
}

bool spHandleLoggerFilenameProperty(char** property, char* value, SP_CONFIG_MSG* msg,
                            const char *filename, int lineNum){
    if(!property || !msg || !filename){
        return false;
    }
    if(!spTurnIntoWord(value)){
        spPrintInvalidValueError(filename, lineNum);
        *msg = SP_CONFIG_INVALID_STRING;
        return false;
    }
    if(strcmp(value, "stdout") == 0){
        *property = NULL;
    }
    else{
        *property = malloc((strlen(value) + 1) * sizeof(char));
        if(!(*property)){
            *msg = SP_CONFIG_ALLOC_FAIL;
            return false;
        }
        snprintf(*property, strlen(value) + 1, "%s", value);
    }
    return true;
}

/**
 * @assert strlen(value) > 0 && value[0] != ' '
 */
bool spHandleIntegerProperty(int* property, bool* propertyCheck, char* value, SP_CONFIG_MSG* msg,
							const char *filename, int lineNum, int lowerBound, int upperBound, bool isThereUpperBound){
	if(!property || !msg || !filename){
		return false;
	}
	if(!spTurnIntoWord(value)){
		spPrintInvalidValueError(filename, lineNum);
		*msg = SP_CONFIG_INVALID_INTEGER;
		return false;
	}
	if(!spIsNonNegativeInteger(value)){
		spPrintInvalidValueError(filename, lineNum);
		*msg = SP_CONFIG_INVALID_INTEGER;
		return false;
	}
	int num = atoi(value);
	if(num >= lowerBound && (!isThereUpperBound || num <= upperBound)){
		*property = num;
		if(propertyCheck){
			*propertyCheck = false;
		}
	}
	else{
		*msg = SP_CONFIG_INVALID_INTEGER;
		spPrintInvalidValueError(filename, lineNum);
		return false;
	}
	return true;
}

/**
 * @assert strlen(value) > 0 && value[0] != ' '
 */
bool spHandleBooleanProperty(bool* property, char* value, SP_CONFIG_MSG* msg, const char *filename,
							 int lineNum){
	if(!property || !msg || !filename){
		return false;
	}
	if(!spTurnIntoWord(value)){
		spPrintInvalidValueError(filename, lineNum);
		*msg = SP_CONFIG_INVALID_STRING;
		return false;
	}
	if(strcmp(value, "true") == 0){
		*property = true;
	}
	else if(strcmp(value, "false") == 0){
		*property = false;
	}
	else{
		*msg = SP_CONFIG_INVALID_STRING;
		spPrintInvalidValueError(filename, lineNum);
		return false;
	}
	return true;
}

/**
 * @assert strlen(value) > 0 && value[0] != ' '
 */
bool spHandleSuffixProperty(char** property, bool* propertyCheck, char* value, SP_CONFIG_MSG* msg, const char *filename,
							 int lineNum){
	if(!property || !propertyCheck || !msg || !filename){
		return false;
	}
	if(!spTurnIntoWord(value)){
		spPrintInvalidValueError(filename, lineNum);
		*msg = SP_CONFIG_INVALID_STRING;
		return false;
	}
	if((strcmp(value, ".jpg") == 0) || (strcmp(value, ".png") == 0) ||
	   (strcmp(value, ".bmp") == 0) || (strcmp(value, ".gif") == 0)){
		*property = malloc((strlen(value) + 1) * sizeof(char));
		if(!(*property)){
			*msg = SP_CONFIG_ALLOC_FAIL;
			return false;
		}
		snprintf(*property, strlen(value) + 1, "%s", value);
		*propertyCheck = false;
		return true;
	}
	*msg = SP_CONFIG_INVALID_STRING;
	spPrintInvalidValueError(filename, lineNum);
	return false;
}

/**
 * @assert strlen(value) > 0 && value[0] != ' '
 */
bool spHandleSplitMethodProperty(SP_KD_SPLIT_MODE* property, char* value, SP_CONFIG_MSG* msg, const char *filename,
							 int lineNum){
	if(!property || !msg || !filename){
		return false;
	}
	if(!spTurnIntoWord(value)){
		spPrintInvalidValueError(filename, lineNum);
		*msg = SP_CONFIG_INVALID_STRING;
		return false;
	}
	if(strcmp(value, "RANDOM") == 0){
		*property = RANDOM;
	}
	else if(strcmp(value, "MAX_SPREAD") == 0){
		*property = MAX_SPREAD;
	}
	else if(strcmp(value, "INCREMENTAL") == 0){
		*property = INCREMENTAL;
	}
	else{
		*msg = SP_CONFIG_INVALID_STRING;
		spPrintInvalidValueError(filename, lineNum);
		return false;
	}
	return true;
}

bool spTryUpdateConfiguration(SPConfig config, char firstArg[1024], char secondArg[1024], bool *isDirectoryMissing,
							  bool *isPrefixMissing, bool *isSuffixMissing, bool *isImageNumMissing, SP_CONFIG_MSG *msg,
							  const char *filename, int lineNum) {
	if(!config || !firstArg || !secondArg || !isDirectoryMissing || !isPrefixMissing || !isSuffixMissing
	   || !isImageNumMissing || !filename){
		return false;
	}
	if(strcmp(firstArg, "spImagesDirectory") == 0){
		return spHandleStringProperty(&(config->imagesDirectory),isDirectoryMissing, secondArg, msg, filename, lineNum);
	}
	else if(strcmp(firstArg, "spImagesPrefix") == 0){
		return spHandleStringProperty(&(config->imagesPrefix), isPrefixMissing, secondArg, msg, filename, lineNum);
	}
	else if(strcmp(firstArg, "spImagesSuffix") == 0){
		return spHandleSuffixProperty(&(config->imagesSuffix), isSuffixMissing,secondArg,msg,filename,lineNum);
	}
	else if(strcmp(firstArg, "spNumOfImages") == 0){
		return spHandleIntegerProperty(&(config->numOfImages), isImageNumMissing, secondArg, msg, filename, lineNum,
									   1, 0, false);
	}
	else if(strcmp(firstArg, "spPCADimension") == 0){
		return spHandleIntegerProperty(&(config->PCADimension), NULL, secondArg, msg, filename, lineNum, 10, 28, true);
	}
	else if(strcmp(firstArg, "spPCAFilename") == 0){
		return spHandleStringProperty(&(config->PCAFilename), NULL, secondArg, msg, filename, lineNum);
	}
	else if(strcmp(firstArg, "spNumOfFeatures") == 0){
		return spHandleIntegerProperty(&(config->numOfFeatures), NULL, secondArg, msg, filename, lineNum, 1, 0, false);
	}
	else if(strcmp(firstArg, "spExtractionMode") == 0){
		return spHandleBooleanProperty(&(config->extractionMode), secondArg, msg, filename, lineNum);
	}
	else if(strcmp(firstArg, "spNumOfSimilarImages") == 0){
		return spHandleIntegerProperty(&(config->numOfSimilarImages), NULL, secondArg, msg, filename, lineNum,
									   1, 0, false);
	}
	else if(strcmp(firstArg, "spKDTreeSplitMethod") == 0){
		return spHandleSplitMethodProperty(&(config->KDTreeSplitMethod), secondArg, msg, filename, lineNum);
	}
	else if(strcmp(firstArg, "spKNN") == 0){
		return spHandleIntegerProperty(&(config->KNN), NULL, secondArg, msg, filename, lineNum, 1, 0, false);
	}
	else if(strcmp(firstArg, "spMinimalGUI") == 0){
		return spHandleBooleanProperty(&(config->minimalGUI), secondArg, msg, filename, lineNum);
	}
	else if(strcmp(firstArg, "spLoggerLevel") == 0){
		return spHandleIntegerProperty(&(config->loggerLevel), NULL, secondArg, msg, filename, lineNum, 1, 4, true);
	}
	else if(strcmp(firstArg, "spLoggerFilename") == 0){
		return spHandleLoggerFilenameProperty(&(config->loggerFilename), secondArg, msg, filename, lineNum);
	}
	*msg = SP_CONFIG_INVALID_STRING;
	spPrintInvalidLineError(filename, lineNum);
	return false;
}

bool spIsLineParsable(char* line, char* firstStr, char* secondStr){
	if (!line || !firstStr || !secondStr){		// if any of the strings NULL then line is incorrect
		return false;
	}
	size_t length = strlen(line);
	size_t i = 0;
	while (i < length && line[i] == ' '){	// pass any spaces
		i++;
	}
	if (i >= length){	// if line ended than it's empty and correct
        snprintf(firstStr, 1, "%s", "");
//		strncpy(firstStr, "", 1); TODO delete if works
		return true;
	}
	if (line[i] == '#'){	// if the first actual character is '#' than this line is comment and ignore it
        snprintf(firstStr, 1, "%s", "");
//		strncpy(firstStr, "", 1); TODO delete if works
		return true;
	}
	size_t index = i;	// this line must be a configuration for a property and follow its order
	size_t n = 0;
	while (i < length && line[i] != ' ' && line[i] != '='){	// determining the first word's numOfPoints
		n++;
		i++;
	}
	if (i >= length){	// if line ended than it's incorrect
		return false;
	}
    snprintf(firstStr, n + 1, "%s", line + index);  // copy first word
//	strncpy(firstStr, line + index, n);	// copy first word TODO delete if works
	if (line[i] == ' '){	// if there are spaces right after the first word
		while (i < length && line[i] == ' '){	// pass any spaces
			i++;
		}
		if (i >= length){	// if line ended than it's incorrect
			return false;
		}
	}
	if(line[i] != '='){	// if the is no '=' character the line is incorrect
		return false;
	}
	i++;
	while (i < length && line[i] == ' '){	// pass any spaces
		i++;
	}
	if (i >= length){ // if line ended than it's incorrect
		return false;
	}
    snprintf(secondStr, length - i + 1, "%s", line + i);  // // copy the second word. assert length > i
//    strncpy(secondStr, line + i, length - i - 1);	// copy the second word. assert length > i TODO delete if works
    return true;

}

/**
 * Creates a new system configuration struct. The configuration struct
 * is initialized based on the configuration file given by 'filename'.
 *
 * @param filename - the name of the configuration file
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains all system configuration.
 *
 * The resulting value stored in msg is as follow:
 * - SP_CONFIG_INVALID_ARGUMENT - if filename == NULL
 * - SP_CONFIG_CANNOT_OPEN_FILE - if the configuration file given by filename cannot be open
 * - SP_CONFIG_ALLOC_FAIL - if an allocation failure occurred
 * - SP_CONFIG_INVALID_INTEGER - if a line in the config file contains invalid integer
 * - SP_CONFIG_INVALID_STRING - if a line in the config file contains invalid string
 * - SP_CONFIG_MISSING_DIR - if spImagesDirectory is missing
 * - SP_CONFIG_MISSING_PREFIX - if spImagesPrefix is missing
 * - SP_CONFIG_MISSING_SUFFIX - if spImagesSuffix is missing
 * - SP_CONFIG_MISSING_NUM_IMAGES - if spNumOfImages is missing
 * - SP_CONFIG_SUCCESS - in case of success
 *
 *
 */
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	if (filename == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	SPConfig config = spDefaultConfigConstructor();
	if (!config) {
		*msg = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}
	FILE *inputFile = fopen(filename, READ_MODE);
	if (inputFile == NULL) {
		free(config);
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		return NULL;
	}
	bool isDirectoryMissing = true, isPrefixMissing = true, isSuffixMissing = true, isImageNumMissing = true;
	char line[MAX_LINE_LENGTH];
	int lineNum = 0;
	while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
		lineNum++;
		char firstArg[MAX_LINE_LENGTH];
		char secondArg[MAX_LINE_LENGTH];
		if (spIsLineParsable(line, firstArg, secondArg)) {
			if (strlen(firstArg) > 0) { // if true than line is configuration line else it's a comment or empty line
				if (!spTryUpdateConfiguration(config, firstArg, secondArg, &isDirectoryMissing, &isPrefixMissing,
											  &isSuffixMissing, &isImageNumMissing, msg, filename, lineNum)) {
                    spConfigDestroy(config);
					free(config);
                    return NULL;
				}
			}
		} else { // line is not parsable
            *msg = SP_CONFIG_INVALID_STRING; // TODO check with Moab if this is the right message for this error
			spPrintInvalidLineError(filename, lineNum);
            spConfigDestroy(config);
			free(config);
            return NULL;
        }
	}
	fclose(inputFile);

	if (isDirectoryMissing) {
		*msg = SP_CONFIG_MISSING_DIR;
        spPrintUndefinedValueError(filename, lineNum, "spImagesDirectory");
		return NULL;
	}
	if (isPrefixMissing) {
		*msg = SP_CONFIG_MISSING_PREFIX;
        spPrintUndefinedValueError(filename, lineNum, "spImagesPrefix");
		return NULL;
	}
	if (isSuffixMissing) {
		*msg = SP_CONFIG_MISSING_SUFFIX;
        spPrintUndefinedValueError(filename, lineNum, "spImagesSuffix");
		return NULL;
	}
	if (isImageNumMissing) {
		*msg = SP_CONFIG_MISSING_NUM_IMAGES;
        spPrintUndefinedValueError(filename, lineNum, "spNumOfImages");
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    }
    *msg = SP_CONFIG_SUCCESS;
    return config->extractionMode;
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    }
    *msg = SP_CONFIG_SUCCESS;
    return config->minimalGUI;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    }
    *msg = SP_CONFIG_SUCCESS;
    return config->numOfImages;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    }
    *msg = SP_CONFIG_SUCCESS;
    return config->numOfFeatures;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    }
    *msg = SP_CONFIG_SUCCESS;
    return config->PCADimension;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index){
    if (imagePath == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    if(index < 0 || index >= config->numOfImages){
        return SP_CONFIG_INDEX_OUT_OF_RANGE;
    }
    sprintf(imagePath, "%s/%s%d%s",config->imagesDirectory, config->imagesPrefix, index, config->imagesSuffix);
    return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetImagePathWithData(char* imagePath, char* directory, char* prefix, int index, char* suffix){
    if (!imagePath || !directory || !prefix || !suffix) {
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    sprintf(imagePath, "%s/%s%d%s",directory, prefix, index, suffix);
    return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
    if (pcaPath == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    sprintf(pcaPath, "%s%s",config->imagesDirectory, config->PCAFilename);
    return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetKDTreeSplitMethod(const SPConfig config, SP_KD_SPLIT_MODE* splitMethod){
	if (splitMethod == NULL || !config) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	*splitMethod = config->KDTreeSplitMethod;
	return SP_CONFIG_SUCCESS;
}


SP_CONFIG_MSG spConfigGetImagesDirectory(const SPConfig config, char* imagesDirectory){
	if (imagesDirectory == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	sprintf(imagesDirectory, "%s", config->imagesDirectory);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetImagesPrefix(const SPConfig config, char* imagesPrefix){
	if (imagesPrefix == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	sprintf(imagesPrefix, "%s", config->imagesPrefix);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetImagesSuffix(const SPConfig config, char* imagesSuffix){
	if (imagesSuffix == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	sprintf(imagesSuffix, "%s", config->imagesSuffix);
	return SP_CONFIG_SUCCESS;
}

int spConfigGetKNN(const SPConfig config, SP_CONFIG_MSG* msg){
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->KNN;
}

int spConfigGetNumOfSimmilarImages(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    }
    *msg = SP_CONFIG_SUCCESS;
    return config->numOfSimilarImages;
}

char* spConfigGetLoggerFilename(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NULL;
    }

    if(!config->loggerFilename){
        *msg = SP_CONFIG_SUCCESS;
        return NULL;
    }
    char* loggerFilename = malloc(MAX_LINE_LENGTH * sizeof(char));
    if(!loggerFilename){
        *msg = SP_CONFIG_ALLOC_FAIL;
        return NULL;
    }
    *msg = SP_CONFIG_SUCCESS;
    sprintf(loggerFilename, "%s", config->loggerFilename);
    return loggerFilename;
}

SP_CONFIG_MSG spConfigGetLoggerLevel(const SPConfig config, int* loggerLevel){
    if (loggerLevel == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    *loggerLevel = config->loggerLevel;
    return SP_CONFIG_SUCCESS;
}


void spConfigDestroy(SPConfig config){
    if(config){
		 if(config->imagesDirectory){
			 free(config->imagesDirectory);
		 }
		if(config->imagesPrefix){
			free(config->imagesPrefix);
		}
		if(config->imagesSuffix){
			free(config->imagesSuffix);
		}
		if(config->PCAFilename){
			free(config->PCAFilename);
		}
		if(config->loggerFilename){
			free(config->loggerFilename);
		}
    }
    free(config);
}