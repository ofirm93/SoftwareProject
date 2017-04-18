/*
 * ExtModeForMain.h
 *
 *  Created on: Apr 17, 2017
 *      Author: user
 */

#ifndef EXTMODEFORMAIN_H_
#define EXTMODEFORMAIN_H_

#include "SPImageProc.h"


/**
 * Using this function to start ExtractionMode.
 * In this mode, we first extract the features of each image and then store each
 * of these features to a file which will be located in the directory given by spImagesDirectory.
 *
 * @param
 * directory - the directory where the images are stored at.
 * imagePrefix - the prefix of the image files.
 * imageSuffix - the suffix of the image files.
 * spNumOfImages - number of images in the directory
 * spNumOfFeatures - number of features to be extracted
 * spIp - special object to help us use SPImageProc's functions
 *
 *
 */
void ExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
		int spNumOfImages, int spNumOfFeatures, sp::ImageProc spIp);


/**
 * Using this function to start Non-ExtractionMode.
 * In this mode, we extract the features from the files we created earlier
 *
 * @param
 * directory - the directory where the images are stored at.
 * imagePrefix - the prefix of the image files.
 * imageSuffix - the suffix of the image files.
 * spNumOfImages - number of images in the directory
 * spNumOfFeatures - number of features to be extracted
 *
 * @return
 * returns the features extracted from the files, for each image on success.
 * Otherwise returns NULL.
 *
 */
SPPoint*** NonExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
		int spNumOfImages, int spNumOfFeatures);


#endif /* EXTMODEFORMAIN_H_ */
