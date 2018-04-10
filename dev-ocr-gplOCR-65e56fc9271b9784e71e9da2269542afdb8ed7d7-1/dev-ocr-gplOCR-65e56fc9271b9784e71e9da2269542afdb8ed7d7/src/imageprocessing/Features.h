#ifndef FEATURES_H
#define FEATURES_H

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "../util/DebugManager.h"
#include "opencv2/legacy/legacy.hpp" 

#include "../util/DebugManager.h"

using namespace cv;
using std::vector;

class Features{
 
 public:
	/**
	 * Create an artificial image of size 512 x 512 having lines in multiple directions and compute Gabor
	 * filter bank  response.
	 */
	static void testGaborFeatures();
	/**
	 * Convert input image into Gray scale (if RGB) and compute Gabor
	 * filter bank  response.
	 */
	static void testGaborFeatures(const char* inpImage);
	static void testGaborFeatures(const cv::Mat& inpImageGray);
	static void getSobelFeatures(const Mat& grey_image, std::vector<float>& sobelFeatures);
	static void splitImageCentroid(const Mat& grey_image, std::vector<Mat>& splitImage);

	static void getHorizontalRunLength(const char* inpImage);
	static void getVerticalRunLength(const char* inpImage);
};
#endif
