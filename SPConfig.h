#ifndef SPCONFIG_H_
#define SPCONFIG_H_

#include <stdbool.h>
#include <stdio.h>
#include "SPLogger.h"

/**
 * A data-structure which is used for configuring the system.
 */

typedef enum sp_config_msg_t {
	SP_CONFIG_MISSING_DIR,
	SP_CONFIG_MISSING_PREFIX,
	SP_CONFIG_MISSING_SUFFIX,
	SP_CONFIG_MISSING_NUM_IMAGES,
	SP_CONFIG_CANNOT_OPEN_FILE,
	SP_CONFIG_ALLOC_FAIL,
	SP_CONFIG_INVALID_INTEGER,
	SP_CONFIG_INVALID_STRING,
	SP_CONFIG_INVALID_ARGUMENT,
	SP_CONFIG_INDEX_OUT_OF_RANGE,
	SP_CONFIG_SUCCESS
} SP_CONFIG_MSG;

typedef enum sp_kdtree_split_mode_t {
	RANDOM,
	MAX_SPREAD,
	INCREMENTAL
} SP_KD_SPLIT_MODE;

typedef struct sp_config_t* SPConfig;

/**
 * A default constructor for SPConfig
 * @return
 * a default SPConfig
 */
SPConfig spDefaultConfigConstructor();

/**
 * A constructor for SPConfig based on the parameters given.
 * @return
 * a SPConfig with the given values
 */
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
                             char *loggerFilename);

/**
 * This method gets two configurations checks if they are the same and if so returns true.
 * @param config
 * @param configExp
 * @return
 * true 	- if the two configurations are valid and the same.
 * false 	- otherwise
 */
bool spIsConfigEqual(SPConfig config, SPConfig configExp);

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
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg);

/**
 * Returns true if spExtractionMode = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Returns true if spMinimalGUI = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Returns the number of images set in the configuration file, i.e the value
 * of spNumOfImages.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Returns the number of features to be extracted. i.e the value
 * of spNumOfFeatures.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Returns the dimension of the PCA. i.e the value of spPCADimension.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Given an index 'index' the function stores in imagePath the full path of the
 * ith image.
 *
 * For example:
 * Given that the value of:
 *  spImagesDirectory = "./images/"
 *  spImagesPrefix = "img"
 *  spImagesSuffix = ".png"
 *  spNumOfImages = 17
 *  index = 10
 *
 * The functions stores "./images/img10.png" to the address given by imagePath.
 * Thus the address given by imagePath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @param index - the index of the image.
 *
 * @return
 * - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 * - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages
 * - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index);
/**
 * This method is the same as the method spConfigGetImagePath() but the properties are given explicitly.
 */
SP_CONFIG_MSG spConfigGetImagePathWithData(char* imagePath, char* directory, char* prefix, int index, char* suffix);

/**
 * The function stores in pcaPath the full path of the pca file.
 * For example given the values of:
 *  spImagesDirectory = "./images/"
 *  spPcaFilename = "pca.yml"
 *
 * The functions stores "./images/pca.yml" to the address given by pcaPath.
 * Thus the address given by pcaPath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @return
 *  - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 *  - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config);

/**
 * A Getter method for the KDTreeSplitMethod.
 * @param config - the configuration to extract from.
 * @param splitMethod - a pointer to assign the requested value.
 * @return
 * SP_CONFIG_INVALID_ARGUMENT 		- if one of the arguments is invalid
 * SP_CONFIG_SUCCESS 				- otherwise
 */
SP_CONFIG_MSG spConfigGetKDTreeSplitMethod(const SPConfig config, SP_KD_SPLIT_MODE* splitMethod);

/**
 * A Getter method for the ImageDirectory.
 * @param config - the configuration to extract from.
 * @param imagesDirectory - a pointer to assign the requested value.
 * @return
 * SP_CONFIG_INVALID_ARGUMENT 		- if one of the arguments is invalid
 * SP_CONFIG_SUCCESS 				- otherwise
 */
SP_CONFIG_MSG spConfigGetImagesDirectory(const SPConfig config, char* imagesDirectory);

/**
 * A Getter method for the ImagePrefix.
 * @param config - the configuration to extract from.
 * @param imagesPrefix - a pointer to assign the requested value.
 * @return
 * SP_CONFIG_INVALID_ARGUMENT 		- if one of the arguments is invalid
 * SP_CONFIG_SUCCESS 				- otherwise
 */
SP_CONFIG_MSG spConfigGetImagesPrefix(const SPConfig config, char* imagesPrefix);

/**
 * A Getter method for the ImageSuffix.
 * @param config - the configuration to extract from.
 * @param imagesSuffix - a pointer to assign the requested value.
 * SP_CONFIG_INVALID_ARGUMENT 		- if one of the arguments is invalid
 * SP_CONFIG_SUCCESS 				- otherwise
 */
SP_CONFIG_MSG spConfigGetImagesSuffix(const SPConfig config, char* imagesSuffix);

/**
 * A Getter method for the KNN.
 * @param config - the configuration to extract from.
 * @param msg - a config message pointer to assign the method's result.
 * @return
 * -1 		- if one of the arguments is invalid. (msg = SP_CONFIG_INVALID_ARGUMENT)
 * KNN 		- otherwise (msg = SP_CONFIG_SUCCESS)
 */
int spConfigGetKNN(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * A Getter method for the LoggerFilename.
 * @param config - the configuration to extract from.
 * @param msg - a config message pointer to assign the method's result.
 * @return
 * NULL 				- if one of the arguments is invalid. (msg = SP_CONFIG_INVALID_ARGUMENT)
 * (msg = SP_CONFIG_ALLOC_FAIL if failed allocating memory)
 * The logger filename 	- otherwise
 */
char* spConfigGetLoggerFilename(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * A Getter method for the LoggerLevel.
 * @param config - the configuration to extract from.
 * @param loggerLevel - a pointer to assign the requested value.
 * @return
 * SP_CONFIG_INVALID_ARGUMENT 		- if one of the arguments is invalid
 * SP_CONFIG_SUCCESS 				- otherwise
 */
SP_CONFIG_MSG spConfigGetLoggerLevel(const SPConfig config, int* loggerLevel);

/**
 * Frees all memory resources associate with config.
 * If config == NULL nothing is done.
 * @param config - the configuration to destroy
 */
void spConfigDestroy(SPConfig config);

/**
 * A Getter method for the NumberOfSimilarImages.
 * @param config - the configuration to extract from.
 * @param msg - a config message pointer to assign the method's result.
 * @return
 * -1 							- if one of the arguments is invalid. (msg = SP_CONFIG_INVALID_ARGUMENT)
 * NumberOfSimilarImages 		- otherwise (msg = SP_CONFIG_SUCCESS)
 */
int spConfigGetNumOfSimilarImages(const SPConfig config, SP_CONFIG_MSG *msg);

#endif /* SPCONFIG_H_ */
