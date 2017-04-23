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

#define MAX_PATH_LENGTH 1024

#define QRY_IMG_MSG "Please enter an image path:\n"
#define EXIT_MSG "Exiting...\n"

#define ERR_MSG_INVALID_ARG "One of the arguments supplied to the method is invalid."
#define ERR_MSG_INVALID_COMM_LINE_ARG "\"Invalid command line arguments : use -c <config_filename>\"\n"
#define ERR_MSG_READ_CONF_FILE "Error while reading the configuration file : "
#define ERR_MSG_MISSING_DIR "Didn't find images directory path."
#define ERR_MSG_MISSING_PREFIX "Didn't find images prefix."
#define ERR_MSG_MISSING_SUFFIX "Didn't find images suffix."
#define ERR_MSG_MISSING_NUM_IMAGES "Didn't find the number of images."
#define ERR_MSG_CANNOT_OPEN_DEFAULT_CONF_FILE "\"The default configuration file spcbir.config couldn’t be open\"\n"
#define ERR_MSG_CANNOT_OPEN_CONF_FILE "\"The configuration file %s couldn’t be open\"\n"
#define ERR_MSG_ALLOC_FAIL "Error : An allocation error happened."
#define ERR_MSG_INVALID_INTEGER "An integer value was expected"
#define ERR_MSG_INVALID_STRING "One string value was expected"
extern "C"{
    #include "SPConfig.h"
    #include "main_aux.h"
    #include "ExtModeForMain.h"
}

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
        config = spConfigCreate("spcbir.config", &configMsg);
    }
    else{
        printf(ERR_MSG_INVALID_COMM_LINE_ARG);
        return 0;
    }
    switch (configMsg){
        case SP_CONFIG_INVALID_ARGUMENT :
            break;
        case SP_CONFIG_MISSING_DIR:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_MISSING_DIR);
            return 0;
        case SP_CONFIG_MISSING_PREFIX:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_MISSING_PREFIX);
            return 0;
        case SP_CONFIG_MISSING_SUFFIX:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_MISSING_SUFFIX);
            return 0;
        case SP_CONFIG_MISSING_NUM_IMAGES:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_MISSING_NUM_IMAGES);
            return 0;
        case SP_CONFIG_CANNOT_OPEN_FILE :
            if(isDefaultConfig){
                printf(ERR_MSG_CANNOT_OPEN_DEFAULT_CONF_FILE);
            }
            else{
                printf(ERR_MSG_CANNOT_OPEN_CONF_FILE, argv[2]);
            }
            return 0;
        case SP_CONFIG_ALLOC_FAIL:
            printf(ERR_MSG_ALLOC_FAIL);
            return 0;
        case SP_CONFIG_INVALID_INTEGER:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_INVALID_INTEGER);
            return 0;
        case SP_CONFIG_INVALID_STRING:
            printf("%s%s",ERR_MSG_READ_CONF_FILE, ERR_MSG_INVALID_STRING);
            return 0;

        case SP_CONFIG_INDEX_OUT_OF_RANGE:break;
        case SP_CONFIG_SUCCESS:break;
    }

    char loggerFilename[MAX_PATH_LENGTH];
    configMsg = spConfigGetLoggerFilename(config, loggerFilename);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        
        return 1;
    }
    int loggerLevelNum;
    configMsg = spConfigGetLoggerLevel(config, &loggerLevelNum);
    switch (configMsg){
        case SP_CONFIG_INVALID_ARGUMENT :
            break;
        case SP_CONFIG_SUCCESS:
            break;
        default:
            //TODO print error
            return 1;
    }
    SP_LOGGER_LEVEL loggerLevel;
    SP_LOGGER_MSG loggerMsg = spLoggerLevelFromNum(loggerLevelNum, &loggerLevel);
    switch (loggerMsg){
        case SP_LOGGER_INVAlID_ARGUMENT :
            break;
        case SP_LOGGER_UNDIFINED:
            break;
        case SP_LOGGER_SUCCESS:
            break;
        default:
            //TODO print error
            return 1;
    }
    loggerMsg = spLoggerCreate(loggerFilename, loggerLevel);
    switch (loggerMsg){
        case SP_LOGGER_DEFINED :
            break;
        case SP_LOGGER_OUT_OF_MEMORY:
            break;
        case SP_LOGGER_CANNOT_OPEN_FILE:
            break;
        case SP_LOGGER_SUCCESS:
            break;
        default:
            //TODO print error
            return 1;
    }
    // TODO from here on every message is to the logger only.
    bool isExtractionMode = spConfigIsExtractionMode(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
        // TODO problem with arguments

        return 2;
    }
    // Get all the fields required to extract features.
    char imagesDirectory[MAX_PATH_LENGTH];
    configMsg = spConfigGetImagesDirectory(config, imagesDirectory);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 3;
    }
    char imagesPrefix[MAX_PATH_LENGTH];
    configMsg = spConfigGetImagesPrefix(config, imagesPrefix);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 4;
    }
    char imagesSuffix[MAX_PATH_LENGTH];
    configMsg = spConfigGetImagesSuffix(config, imagesSuffix);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 5;
    }
    int numOfImages = spConfigGetNumOfImages(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 6;
    }
    int numOfFeatures = spConfigGetNumOfFeatures(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 7;
    }
    int dim = spConfigGetPCADim(config, &configMsg);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 8;
    }
    SP_KD_SPLIT_MODE kdSplitMode;
    configMsg = spConfigGetKDTreeSplitMethod(config, &kdSplitMode);
    if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 9;
    }
    SPPoint*** features;

    if(isExtractionMode){
        // TODO extract features
        sp::ImageProc s = sp::ImageProc(config);
        // TODO bar should alter the method so it would be correct
        features = ExtractionModeAct(imagesDirectory, imagesPrefix, imagesSuffix, numOfImages, numOfFeatures, s);

    } else{
        features = NonExtractionModeAct(imagesDirectory, imagesPrefix, imagesSuffix, numOfImages, numOfFeatures);
    }
    if(!features){
        // TODO Error in extracting features
        return 8;
    }

    SPPoint** gallery = NULL; // TODO flatten features from 3d to only 2d array
    int gallerySize = 0; // TODO Get features number
    SPKDTree* tree = spInitSPKDTree(gallery, gallerySize, dim, kdSplitMode);
    bool isEnded = false;
    char* queryStr;
    while(!isEnded){
        queryStr = spGetInputFromUser(QRY_IMG_MSG);
        if(strcmp(queryStr, "<>") == 0){
            printf(EXIT_MSG);
            isEnded = true;
        } else{
            int* KNN = spGetGetBestKMatches(tree, queryStr, config);
            if(KNN){
                bool isGUIMode = spConfigMinimalGui(config, &configMsg);
                if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
                    // TODO problem with arguments
                    //TODO free memory
                    return 10;
                }
                int k = spConfigGetKNN(config, &configMsg);
                if(configMsg == SP_CONFIG_INVALID_ARGUMENT){
                    // TODO problem with arguments
                    // TODO free all resources
                    return 11;
                }
                if(isGUIMode){
                    spMinimalGUI(config,queryStr,KNN,k);
                }
                else{
                    spNonMinimalGUI(config, queryStr, KNN, k);
                }
            }
        }
        free(queryStr);
    }
    return 0;
}