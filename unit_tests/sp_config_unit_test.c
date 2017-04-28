#include "../SPConfig.h"
#include "unit_test_util.h"

#include <stdlib.h>
#include <string.h>

#define FAIL_TEST_FILENAME_FORMAT "./unit_tests/config_files/fail_test%d.config"
//
// Created by Ofir on 09/03/2017.
//

//bool spIsStringsEqual(char *paramName, char *str, char *strExp){
//    if(strcmp(str,strExp) != 0){
//        printf("[Ofir_Test] The parameter %s in the created config is %s while the expected is %s", paramName,str, strExp);
//        return false;
//    }
//    return true;
//}
//
//bool spIsNumbersEqual(char *paramName, int num, int numExp){
//    if(num != numExp){
//        printf("[Ofir_Test] The parameter %s in the created config is %d while the expected is %d", paramName,num, numExp);
//        return false;
//    }
//    return true;
//}

//bool spIsConfigEqual(SPConfig config, SPConfig configExp) {
//    if(!config || !configExp){
//        printf("[Ofir_Test] Null pointer error in the test itself");
//        return false;
//    }
//    if((!spIsStringsEqual("imagesDirectory", config->imagesDirectory, configExp->imagesDirectory)) ||
//            (!spIsStringsEqual("imagesPrefix", config->imagesPrefix, configExp->imagesPrefix)) ||
//            (!spIsStringsEqual("imagesSuffix", config->imagesSuffix, configExp->imagesSuffix)) ||
//            (!spIsNumbersEqual("numOfImages", config->numOfImages, configExp->numOfImages)) ||
//            (!spIsNumbersEqual("PCADimension", config->PCADimension, configExp->PCADimension)) ||
//            (!spIsStringsEqual("PCAFilename", config->PCAFilename, configExp->PCAFilename)) ||
//            (!spIsNumbersEqual("numOfFeatures", config->numOfFeatures, configExp->numOfFeatures)) ||
//            (!spIsNumbersEqual("extractionMode", config->extractionMode, configExp->extractionMode)) ||
//            (!spIsNumbersEqual("numOfSimilarImages", config->numOfSimilarImages, configExp->numOfSimilarImages)) ||
//            (!spIsNumbersEqual("KDTreeSplitMethod", config->KDTreeSplitMethod, configExp->KDTreeSplitMethod)) ||
//            (!spIsNumbersEqual("KNN", config->KNN, configExp->KNN)) ||
//            (!spIsNumbersEqual("minimalGUI", config->minimalGUI, configExp->minimalGUI)) ||
//            (!spIsNumbersEqual("loggerLevel", config->loggerLevel, configExp->loggerLevel)) ||
//            (!spIsStringsEqual("loggerFilename", config->loggerFilename, configExp->loggerFilename))
//            ){
//        return false;
//    }
//    return true;
//}

bool spTestGoodConfigFile(){
    char filename[] = "./unit_tests/config_files/good_config.config";
    SPConfig expectedConfig = spConfigConstructor("./images/", "img", ".png", 17, 20, "pca.yml", 100, true, 5, MAX_SPREAD, 1, false, 3, "stdout");
    SP_CONFIG_MSG msg;
    SPConfig config = spConfigCreate(filename, &msg);
    if(msg == SP_CONFIG_SUCCESS){
        if(!spIsConfigEqual(config, expectedConfig)){
            spLoggerPrintDebug("Debug : The configuration file built was different from the expected one.", __FILE__, __func__, __LINE__);
            return false;
        }
    }
    else{
        spLoggerPrintDebug("Debug : Failed building the good configuration.", __FILE__, __func__, __LINE__);
        return false;
    }
    return true;
}

bool spTestBadConfig(){
    char filename[] = "./unit_tests/config_files/fail_test%d.config";
    for (int i = 1; i <= 22; ++i) {
        int checker = sprintf(filename, FAIL_TEST_FILENAME_FORMAT, i);
        if(checker < 0){
            spLoggerPrintDebug("Debug : The filename of the bad configuration file wasn't well formatted.", __FILE__, __func__, __LINE__);
            return false;
        }
        SP_CONFIG_MSG msg;
        SPConfig config = spConfigCreate(filename, &msg);
        if(msg != SP_CONFIG_SUCCESS){
            return true;
        }
        else{
            spLoggerPrintDebug("Debug : The bad config didn't failed to be built.", __FILE__, __func__, __LINE__);
            return false;
        }
        return true;
    }
}
bool spTestFile(char* filename, SPConfig expectedConfig, SP_CONFIG_MSG expectedMsg){
    SP_CONFIG_MSG* msg = malloc(sizeof(SP_CONFIG_MSG));
    if(!msg){
        printf("[Ofir_Test] Allocation error in spTestFile()");
        return false;
    }
    SPConfig config = spConfigCreate(filename, msg);
    if(*msg == SP_CONFIG_SUCCESS){
        if(!spIsConfigEqual(config, expectedConfig)){
            printf("[Ofir_Test] The config created by the file is different from the one expected");
            return false;
        }
    }
    return *msg == expectedMsg;
}

//SPConfig spBuildConfig(char *imagesDirectory,
//                       char *imagesPrefix,
//                       char *imagesSuffix,
//                       int numOfImages,
//                       int PCADimension,
//                       char *PCAFilename,
//                       int numOfFeatures,
//                       bool extractionMode,
//                       int numOfSimilarImages,
//                       SP_KD_SPLIT_MODE KDTreeSplitMethod,
//                       int KNN,
//                       bool minimalGUI,
//                       int loggerLevel,
//                       char *loggerFilename) {
//    SPConfig config = spDefaultConfigConstructor();
//    if(!config){
//        printf("[Ofir_Test] Allocation error in spBuildConfig()");
//        return NULL;
//    }
//    config->imagesDirectory = imagesDirectory;
//    config->imagesPrefix = imagesPrefix;
//    config->imagesSuffix = imagesSuffix;
//    config->numOfImages = numOfImages;
//    config->PCADimension = PCADimension;
//    config->PCAFilename = PCAFilename;
//    config->numOfFeatures = numOfFeatures;
//    config->extractionMode = extractionMode;
//    config->numOfSimilarImages = numOfSimilarImages;
//    config->KDTreeSplitMethod = KDTreeSplitMethod;
//    config->KNN = KNN;
//    config->minimalGUI = minimalGUI;
//    config->loggerLevel = loggerLevel;
//    config->loggerFilename = loggerFilename;
//}

/**
 * The way to use the test is to change the number in the first line to the correct test,
 * change the second line to the correct configuration that expected (only if the configuration file is correct),
 * change the third line SP_CONFIG_MSG to the expected message.
 * @return
 */
int main(){
    RUN_TEST(spTestGoodConfigFile);
    RUN_TEST(spTestBadConfig);
    return 0;
}
