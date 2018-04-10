/*
 * ImageAnnotator.h
 *
 *  Created on: 21-Jun-2016
 *      Author: kompalli
 */

#ifndef IMAGEANNOTATOR_H_
#define IMAGEANNOTATOR_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

class ImageAnnotator{
	Point ulhcPoint, lrhcPoint;
	bool pointSelectionStarted;
	Mat originalImage;
	static const std::string windowName;
	int scrolWidth, scrolHight;
	static const int offsetX, offsetY;
	int winH, winW;
	int saveImageIndex;
	std::string outputFolder;

public:
	void annotateImage(Mat& inputImage, std::string outputFolderPath);
	static void putROIInFile(int event, int x, int y, int flags, void* userdata);
	void updateScrollImage(Mat& inpImage);
	void saveImageToFolder();
};





#endif /* IMAGEANNOTATOR_H_ */
