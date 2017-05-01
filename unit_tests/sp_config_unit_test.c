//
// Created by Ofir on 09/03/2017.
//

#include "../SPConfig.h"
#include "unit_test_util.h"

#include <stdlib.h>

#define FAIL_TEST_FILENAME_FORMAT "./unit_tests/config_files/fail_test%d.config"

/**
 * A test for a good configuration extraction.
 * @return true if successful otherwise false
 */
bool spTestGoodConfigFile(){
    char filename[] = "./unit_tests/config_files/good_config.config";
    SPConfig expectedConfig = spConfigConstructor("./unit_tests/RotatedEx3plusProj", "img", ".png", 17, 20, "pca.yml", 100, true, 5, MAX_SPREAD, 1, false, 3, "stdout");
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
/**
 * A fail test for configuration extraction.
 * @return true if successful otherwise false
 */
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
        if(msg == SP_CONFIG_SUCCESS){
            spLoggerPrintDebug("Debug : The bad config didn't failed to be built.", __FILE__, __func__, __LINE__);
            return false;
        }
        spConfigDestroy(config);
    }
    return true;
}


/*bool spTestFile(char* filename, SPConfig expectedConfig, SP_CONFIG_MSG expectedMsg){
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
}*/

int main(){
    RUN_TEST(spTestGoodConfigFile);
    RUN_TEST(spTestBadConfig);
    return 0;
}
