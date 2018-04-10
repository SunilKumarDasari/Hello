/*
 * ImageAnnotator.cpp
 *
 *  Created on: 21-Jun-2016
 *      Author: kompalli
 */
#include "ImageAnnotator.h"



const std::string ImageAnnotator::windowName = "Select ROI";
const int ImageAnnotator::offsetX = 20;
const int ImageAnnotator::offsetY = 20;
//const int ImageAnnotator::winH=300;
//const int ImageAnnotator::winW=600;

void ImageAnnotator::annotateImage(Mat& inputImage, std::string outputFolderPath){
	outputFolder = outputFolderPath;
	pointSelectionStarted = false;
	//namedWindow(windowName, 1);
	originalImage = inputImage.clone();
	namedWindow(windowName, 1);

	winH=640;
	winW=1024;

	if(winH>=inputImage.rows)winH=inputImage.rows-1;
	if(winW>=inputImage.cols)winW=inputImage.cols-1;
	scrolWidth = 0;
	scrolHight = 0;
	saveImageIndex = 0;
	while (1) {
		setMouseCallback(windowName, ImageAnnotator::putROIInFile, this);
		updateScrollImage(originalImage);
		char key = waitKey(0);
		//std::cout << "\n key:" << (int) key;
		if (key == 'x') {
			return;
		}
		if (key == (char) 81){// Left key
			if (scrolWidth > 0){
				scrolWidth -= offsetX;
			}
		}
		if (key == (char) 82){// Up key
			if (scrolHight > 0){
				scrolHight -= offsetY;
			}
		}
		if (key == (char) 83){// Right key
			if (scrolWidth < inputImage.cols - winW){
				scrolWidth += offsetX;
			}

		}
		if (key == (char) 84){// Down key
			if (scrolHight < inputImage.rows - winH){
				scrolHight += offsetY;
			}
		}
	}
}

void ImageAnnotator::updateScrollImage(Mat& inpImage){
	if (scrolWidth + winW >= inpImage.cols || scrolHight + winH >= inpImage.rows || scrolWidth < 0 || scrolHight <0)
		return;
	Mat winImage= inpImage( Rect(scrolWidth,scrolHight,winW,winH) );
	imshow(windowName, winImage);
}


void ImageAnnotator::putROIInFile(int event, int x, int y, int flags,
			void* userdata) {
	ImageAnnotator* currAnnotator = (ImageAnnotator*) userdata;
	int newX = x + currAnnotator->scrolWidth;
	int newY = y + currAnnotator->scrolHight;

	if (event == EVENT_LBUTTONDOWN && !currAnnotator->pointSelectionStarted) {
		currAnnotator->pointSelectionStarted = true;
		currAnnotator->ulhcPoint = Point(newX, newY);
	}

	if (event == CV_EVENT_MOUSEMOVE && currAnnotator->pointSelectionStarted)
	{
		/* mouse dragged. ROI being selected */
		cv::Mat img1 = currAnnotator->originalImage.clone();
		cv::Point point2 = cv::Point(newX, newY);
		cv::rectangle(img1, currAnnotator->ulhcPoint, point2, CV_RGB(255, 0, 0), 3, 8, 0);

		currAnnotator->updateScrollImage(img1);
	}

	if (event == CV_EVENT_LBUTTONUP && currAnnotator->pointSelectionStarted)
	{
		currAnnotator->pointSelectionStarted = false;
		cv::Point point2 = cv::Point(newX, newY);
		char outImagePath[1024];
		sprintf(outImagePath, "%s/result_%03d_%d.%d_%d.%d.jpeg", currAnnotator->outputFolder.c_str(), currAnnotator->saveImageIndex++, currAnnotator->ulhcPoint.x, currAnnotator->ulhcPoint.y, newX, newY);
		cv::Mat img2 = currAnnotator->originalImage(Rect(currAnnotator->ulhcPoint.x, currAnnotator->ulhcPoint.y, newX-currAnnotator->ulhcPoint.x, newY-currAnnotator->ulhcPoint.y));
		imwrite(outImagePath, img2);

		cv::rectangle(currAnnotator->originalImage, currAnnotator->ulhcPoint, point2, CV_RGB(0, 255, 0), 2, 8, 0);
		currAnnotator->updateScrollImage(currAnnotator->originalImage);
	}
}
