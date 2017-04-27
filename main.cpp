//============================================================================
// Name        : SPCBIR.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "ExtModeForMain.h"
#include "main_aux.h"

#define MAX_PATH_LENGTH 1024
#define MAX_ERR_MSG_LENGTH 1024

#define QRY_IMG_MSG "Please enter an image path:\n"
#define EXIT_MSG "Exiting...\n"

#define ERR_MSG_INVALID_ARG "Error : One of the arguments supplied to the method %s is invalid."
#define ERR_MSG_INVALID_COMM_LINE_ARG "\"Invalid command line arguments : use -c <config_filename>\"\n"
#define ERR_MSG_READ_CONF_FILE "Error while reading the configuration file : "
#define ERR_MSG_MISSING_DIR "Didn't find images directory path."
#define ERR_MSG_MISSING_PREFIX "Didn't find images prefix."
#define ERR_MSG_MISSING_SUFFIX "Didn't find images suffix."
#define ERR_MSG_MISSING_NUM_IMAGES "Didn't find the number of images."
#define ERR_MSG_CANNOT_OPEN_DEFAULT_CONF_FILE "\"The default configuration file spcbir.config couldn’t be open\"\n"
#define ERR_MSG_CANNOT_OPEN_CONF_FILE "\"The configuration file %s couldn’t be open\"\n"
#define ERR_MSG_ALLOC_FAIL "Error : An allocation error happened "
#define ERR_MSG_INVALID_INTEGER "An integer value was expected"
#define ERR_MSG_INVALID_STRING "One string value was expected"
#define ERR_MSG_GENERAL "General error. Unexpectedly, the program reached to a code where it shouldn't supposed to.\n"
#define ERR_MSG_CONFIG_INVALID_ARGUMENT "Error : The configuration file wasn't given to the method"
#define ERR_MSG_INVALID_ARGUMENT_LOGGER_FILENAME "Error : One of the arguments given to the method spConfigGetLoggerFilename() is invalid."
#define ERR_MSG_INVALID_ARGUMENT_LOGGER_LEVEL_NUM "Error : One of the arguments given to the method spConfigGetLoggerLevel() is invalid."
#define ERR_MSG_INVALID_ARGUMENT_LOGGER_LEVEL_FROM_NUM "Error : One of the arguments given to the method spLoggerLevelFromNum() is invalid."
#define ERR_MSG_CREATE_CONF "in the method spConfigCreate()"
#define ERR_MSG_CREATE_LOGGER "in the method spLoggerCreate()"
#define ERR_MSG_CANNOT_OPEN_LOGGER_FILE "Error : couldn't open logger file."
#define ERR_MSG_GET_IMG_DIR "spConfigGetImagesDirectory()"
#define ERR_MSG_IS_EXT_MODE "spConfigIsExtractionMode()"
#define ERR_MSG_GET_IMG_PREFIX "spConfigGetImagesPrefix()"
#define ERR_MSG_GET_ING_SUFFIX "spConfigGetImagesSuffix()"
#define ERR_MSG_GET_IMG_NUM "spConfigGetNumOfImages()"
#define ERR_MSG_GET_FEAT_NUM "spConfigGetNumOfFeatures()"
#define ERR_MSG_GET_PCA_DIM "spConfigGetPCADim()"
#define ERR_MSG_GET_SPLIT_METHOD "spConfigGetKDTreeSplitMethod()"
#define ERR_MSG_CANNOT_OBTAIN_FEAT "Error : couldn't obtain features correctly."
#define ERR_MSG_CANNOT_CREATE_KDTREE "Error : Couldn't create KDTree from the features."
#define ERR_MSG_IS_MIN_GUI "spConfigMinimalGui()"
#define ERR_MSG_GET_KNN "spConfigGetKNN()"

using namespace std;

int main(int argc, const char* argv[]) {
    SP_CONFIG_MSG configMsg;
    bool isDefaultConfig = true;
    SPConfig config;
    if(argc == 3 && strcmp(argv[1], "-c") == 0){
        isDefaultConfig = false;
        config = spConfigCreate(argv[2],&configMsg);
    }
    else if(argc == 1){
        config = spConfigCreate("./spcbir.config", &configMsg);
    }
    else{
        printf(ERR_MSG_INVALID_COMM_LINE_ARG);
        return 0;
    }
    switch (configMsg){
        case SP_CONFIG_INVALID_ARGUMENT :
            printf(ERR_MSG_CONFIG_INVALID_ARGUMENT);
            free(config);
            return 0;
        case SP_CONFIG_MISSING_DIR:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_MISSING_DIR);
            free(config);
            return 0;
        case SP_CONFIG_MISSING_PREFIX:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_MISSING_PREFIX);
            free(config);
            return 0;
        case SP_CONFIG_MISSING_SUFFIX:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_MISSING_SUFFIX);
            free(config);
            return 0;
        case SP_CONFIG_MISSING_NUM_IMAGES:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_MISSING_NUM_IMAGES);
            free(config);
            return 0;
        case SP_CONFIG_CANNOT_OPEN_FILE :
            if(isDefaultConfig){
                printf(ERR_MSG_CANNOT_OPEN_DEFAULT_CONF_FILE);
            }
            else{
                printf(ERR_MSG_CANNOT_OPEN_CONF_FILE, argv[2]);
            }
            free(config);
            return 0;
        case SP_CONFIG_ALLOC_FAIL:
            printf("%s%s", ERR_MSG_ALLOC_FAIL, ERR_MSG_CREATE_CONF);
            free(config);
            return 0;
        case SP_CONFIG_INVALID_INTEGER:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_INVALID_INTEGER);
            free(config);
            return 0;
        case SP_CONFIG_INVALID_STRING:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_INVALID_STRING);
            free(config);
            return 0;
        case SP_CONFIG_SUCCESS:
            break;
        default:
            printf(ERR_MSG_GENERAL);
            free(config);
            return 0;
    }
    char loggerFilename[MAX_PATH_LENGTH];
    configMsg = spConfigGetLoggerFilename(config, loggerFilename);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        printf(ERR_MSG_INVALID_ARGUMENT_LOGGER_FILENAME);
        free(config);
        return 1;
    }
    int loggerLevelNum;
    configMsg = spConfigGetLoggerLevel(config, &loggerLevelNum);
    switch (configMsg){
        case SP_CONFIG_INVALID_ARGUMENT :
            printf(ERR_MSG_INVALID_ARGUMENT_LOGGER_LEVEL_NUM);
            free(config);
            return 1;
        case SP_CONFIG_SUCCESS:
            break;
        default:
            printf(ERR_MSG_GENERAL);
            free(config);
            return 1;
    }
    SP_LOGGER_LEVEL loggerLevel;
    SP_LOGGER_MSG loggerMsg = spLoggerLevelFromNum(loggerLevelNum, &loggerLevel);
    switch (loggerMsg){
        case SP_LOGGER_INVAlID_ARGUMENT :
            printf(ERR_MSG_INVALID_ARGUMENT_LOGGER_LEVEL_FROM_NUM);
            free(config);
            return 1;
        case SP_LOGGER_SUCCESS:
            break;
        default:
            printf(ERR_MSG_GENERAL);
            free(config);
            return 1;
    }
    loggerMsg = spLoggerCreate(loggerFilename, loggerLevel);
    switch (loggerMsg){
        case SP_LOGGER_DEFINED :
            break;
        case SP_LOGGER_OUT_OF_MEMORY:
            printf("%s%s", ERR_MSG_ALLOC_FAIL, ERR_MSG_CREATE_LOGGER);
            free(config);
            return 1;
        case SP_LOGGER_CANNOT_OPEN_FILE:
            printf(ERR_MSG_CANNOT_OPEN_LOGGER_FILE);
            free(config);
            return 1;
        case SP_LOGGER_SUCCESS:
            break;
        default:
            printf(ERR_MSG_GENERAL);
            free(config);
            return 1;
    }
    // TODO from here on every message is to the logger only.
    bool isExtractionMode = spConfigIsExtractionMode(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_IS_EXT_MODE);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 2;
    }
    // Get all the fields required to extract features.
    char imagesDirectory[MAX_PATH_LENGTH];
    configMsg = spConfigGetImagesDirectory(config, imagesDirectory);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_GET_IMG_DIR);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 3;
    }
    char imagesPrefix[MAX_PATH_LENGTH];
    configMsg = spConfigGetImagesPrefix(config, imagesPrefix);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_GET_IMG_PREFIX);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 4;
    }
    char imagesSuffix[MAX_PATH_LENGTH];
    configMsg = spConfigGetImagesSuffix(config, imagesSuffix);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_GET_ING_SUFFIX);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 5;
    }
    int numOfImages = spConfigGetNumOfImages(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_GET_IMG_NUM);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 6;
    }

    int dim = spConfigGetPCADim(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_GET_PCA_DIM);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 8;
    }
    SP_KD_SPLIT_MODE kdSplitMode;
    configMsg = spConfigGetKDTreeSplitMethod(config, &kdSplitMode);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_GET_SPLIT_METHOD);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 9;
    }

    bool isGUIMode = spConfigMinimalGui(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_IS_MIN_GUI);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 10;
    }
    int k = spConfigGetNumOfSimmilarImages(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_INVALID_ARG, ERR_MSG_GET_KNN);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 11;
    }

    SPPoint** features;
    int totalNumOfFeatures = 0;
    if(isExtractionMode){
        sp::ImageProc s = sp::ImageProc(config);
        features = ExtractionModeAct(imagesDirectory, imagesPrefix, imagesSuffix, numOfImages, s, &totalNumOfFeatures);
    } else{
        features = NonExtractionModeAct(imagesDirectory, imagesPrefix, numOfImages, &totalNumOfFeatures);
    }
    if(!features){
        spLoggerPrintError(ERR_MSG_CANNOT_OBTAIN_FEAT, __FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return 8;
    }
    // TODO from this point destroy features also
    SPKDTree* tree = spInitSPKDTree(features, totalNumOfFeatures, dim, kdSplitMode);
    if(!tree){
        spLoggerPrintError(ERR_MSG_CANNOT_CREATE_KDTREE, __FILE__, __func__, __LINE__);
        spDestroySPPointArray(features, totalNumOfFeatures);
        spLoggerDestroy();
        free(config);
        return 8;
    }
    // TODO from this point destroy tree also
    bool isEnded = false;
    char* queryStr;
    while(!isEnded){
        queryStr = spGetInputFromUser(QRY_IMG_MSG);
        if(!queryStr){
            spDestroyKDTree(tree);
            spDestroySPPointArray(features, totalNumOfFeatures);
            spLoggerDestroy();
            free(config);
            return 9;
        }
        if(strcmp(queryStr, "<>") == 0){
            printf(EXIT_MSG);
            isEnded = true;
        } else{
            int* KNN = spGetGetBestKMatches(tree, queryStr, config, numOfImages, k);
            if(KNN){
                if(isGUIMode){
                    spMinimalGUI(config,queryStr,KNN,k);
                }
                else{
                    spNonMinimalGUI(config, queryStr, KNN, k);
                }
            }
            else{
                spDestroyKDTree(tree);
                spDestroySPPointArray(features, totalNumOfFeatures);
                spLoggerDestroy();
                free(config);
                return 9;
            }
            free(KNN);
        }
        free(queryStr);
    }

    spDestroyKDTree(tree);
    spDestroySPPointArray(features, totalNumOfFeatures);
    spLoggerDestroy();
    free(config);
    return 0;
}

/*using namespace cv; TODO remove when ending debugging
using namespace std;
#include <unistd.h>

int main(int argc, const char* argv[]) {
    Mat img = imread("../unit_tests/RotatedEx3plusProj/imag0.png", IMREAD_GRAYSCALE);
    if (img.empty()) {
        printf("Error");
    }
}*/

