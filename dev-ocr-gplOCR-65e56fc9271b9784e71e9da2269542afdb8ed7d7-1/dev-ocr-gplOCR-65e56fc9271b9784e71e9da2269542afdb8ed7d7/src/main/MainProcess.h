/*
 * MainProcess.h
 *
 *  Created on: 31-Oct-2015
 *      Author: kompalli
 */

#ifndef MAINPROCESS_H_
#define MAINPROCESS_H_


#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string.h>
#include <sstream>

#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/legacy/legacy.hpp"

#include "../imageprocessing/Features.h"

class MainProcess{
public:
	static void printUsage();
	static int extractSobelBatch(const char* inpImageFileList, const char* imageDirectory, const char* featuresFile);
};


#endif /* MAINPROCESS_H_ */
