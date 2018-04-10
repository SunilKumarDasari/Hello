#include "Features.h"


using namespace cv;
void Features::testGaborFeatures(const char* inpImagePath){
	cv::Mat image, image_gray;
	image = cv::imread(inpImagePath, CV_LOAD_IMAGE_COLOR);
	cv::cvtColor(image, image_gray, CV_BGR2GRAY);
	testGaborFeatures(image_gray);
}

void Features::testGaborFeatures(){
	int imageDim = 513;
	int centerPt = imageDim/2;
	int quarterPt = imageDim/4;
	cv::Mat grayscaleMat(cv::Size(imageDim, imageDim), CV_8U, cv::Scalar(0));

	if (true){
		//Straight line.
		cv::Point startPt(centerPt, 0);
		cv::Point endPt(centerPt, imageDim);
		cv::Scalar colorLine(255);
		int thickness = 1;
		cv::line(grayscaleMat, startPt, endPt, colorLine, thickness);
	}
	if (true){
		//25 degree line.
		cv::Point startPt(0, centerPt+quarterPt);
		cv::Point endPt(imageDim, quarterPt);
		cv::Scalar colorLine(255);
		int thickness = 1;
		cv::line(grayscaleMat, startPt, endPt, colorLine, thickness);
	}
	if (true){
		//45 degree line.
		cv::Point startPt(imageDim, 0);
		cv::Point endPt(0, imageDim);
		cv::Scalar colorLine(255);
		int thickness = 1;
		cv::line(grayscaleMat, startPt, endPt, colorLine, thickness);
	}
	if (true){
		//75 degree line.
		cv::Point startPt(centerPt+quarterPt, 0);
		cv::Point endPt(quarterPt, imageDim);
		cv::Scalar colorLine(255);
		int thickness = 1;
		cv::line(grayscaleMat, startPt, endPt, colorLine, thickness);
	}
	if (true){
		//115 degree line.
		cv::Point startPt(quarterPt, 0);
		cv::Point endPt(centerPt+quarterPt, imageDim);
		cv::Scalar colorLine(255);
		int thickness = 1;
		cv::line(grayscaleMat, startPt, endPt, colorLine, thickness);
	}
	if (true){
		//135 degree line.
		cv::Point startPt(0, 0);
		cv::Point endPt(imageDim, imageDim);
		cv::Scalar colorLine(255);
		int thickness = 1;
		cv::line(grayscaleMat, startPt, endPt, colorLine, thickness);
	}
	if (true){
		//160 degree line.
		cv::Point startPt(0, quarterPt);
		cv::Point endPt(imageDim, centerPt+quarterPt);
		cv::Scalar colorLine(255);
		int thickness = 1;
		cv::line(grayscaleMat, startPt, endPt, colorLine, thickness);
	}
	if (true){
		//Horizontal line.
		cv::Point startPt(0, centerPt);
		cv::Point endPt(imageDim, centerPt);
		cv::Scalar colorLine(255);
		int thickness = 1;
		cv::line(grayscaleMat, startPt, endPt, colorLine, thickness);
	}

	testGaborFeatures(grayscaleMat);

}

void Features::testGaborFeatures(const cv::Mat& inpImageGray){
	//https://github.com/juancamilog/gpu_convolve_test/blob/master/gpu_convolve_test.cpp
	//https://github.com/Itseez/opencv/blob/master/modules/imgproc/src/gabor.cpp
	int kernel_size = 5;
	double lambda = kernel_size;
	//double lambda = 16.0;
	//double sigma = 0.5*lambda;
	double sigma = 2;
	int angle = 180;
	double theta = ((double) angle)*M_PI/180.0;
	double gamma = 1.0;
	double phaseOffset=M_PI*0.5;
	int kernelCoefficientType=CV_32F;

	cv::Mat gaborKernelSine = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sigma, theta, lambda, gamma, phaseOffset, kernelCoefficientType);
	cv::Mat gaborKernelCos = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sigma, theta, lambda, gamma, 0.0, kernelCoefficientType);

	cv::Mat image_f, sin_response, cos_response, temp;

	inpImageGray.convertTo(image_f, CV_32F, 1.0/256.0);
	cv::filter2D(image_f,sin_response, -1, gaborKernelSine, cv::Point(-1,-1));
	cv::filter2D(image_f,cos_response, -1, gaborKernelCos, cv::Point(-1,-1));

	char outPath[1024];

	sprintf(outPath, "%s/0_origImageGray.jpg", DebugManager::getDebugDirectoryPath());
	imwrite(outPath, inpImageGray);


	if (! DebugManager::noDebugImage("FeaturesTracking::testGaborFeatures_2")){
		sprintf(outPath, "%s/1_Kernel_%02d_theta_%03d_sineResult.yml", DebugManager::getDebugDirectoryPath(), kernel_size, angle);
		imwrite(outPath, gaborKernelSine);
		imwrite(outPath, sin_response);
	}

	cv::Mat imageOutSine;
	double minMat, maxMat;
	cv::minMaxLoc(sin_response, &minMat, &maxMat);
	sin_response -= minMat;
	sin_response.convertTo(imageOutSine, CV_8U,255.0/(maxMat-minMat));
	sprintf(outPath, "%s/1_Kernel_%02d_theta_%03d_sineResultGabor.jpg", DebugManager::getDebugDirectoryPath(), kernel_size, angle);
	imwrite(outPath, imageOutSine);

	cv::minMaxLoc(gaborKernelSine, &minMat, &maxMat);
	gaborKernelSine -= minMat;
	gaborKernelSine.convertTo(imageOutSine, CV_8U,255.0/(maxMat-minMat));
	sprintf(outPath, "%s/1_Kernel_%02d_theta_%03d_filter.jpg", DebugManager::getDebugDirectoryPath(), kernel_size, angle);
	imwrite(outPath, imageOutSine);

	if (! DebugManager::noDebugImage("FeaturesTracking::testGaborFeatures_2")){
		sprintf(outPath, "%s/1_Kernel_%02d_theta_%03d_cosResult.yml", DebugManager::getDebugDirectoryPath(), kernel_size, angle);
		//ImageAnnotator::writeImageToDebug("FeaturesTracking::testGaborFeatures", );
		//ImageAnnotator::writeImageToDebug("FeaturesTracking::testGaborFeatures", outPath, cos_response);

		imwrite(outPath, gaborKernelCos);
		imwrite(outPath, cos_response);
	}

	cv::Mat imageOutCos;
	cv::minMaxLoc(cos_response, &minMat, &maxMat);
	cos_response -= minMat;
	cos_response.convertTo(imageOutCos, CV_8U,255.0/(maxMat-minMat));
	sprintf(outPath, "%s/1_Kernel_%02d_theta_%03d_cosResultGabor.jpg", DebugManager::getDebugDirectoryPath(), kernel_size, angle);
	imwrite(outPath, imageOutCos);

}

void Features::getSobelFeatures(const Mat& grey_image, std::vector<float>& sobelFeatures){
	//int size[3] = {grey_image.rows, grey_image.cols, 8};
	//sobelResult = Mat(3, size, CV_32FC1, cvScalar(0));
	Mat srcImageGray = grey_image.clone();
	//GaussianBlur( srcImageGray, srcImageGray, Size(3,3), 0, 0, BORDER_DEFAULT );

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat magnitude, angle;

	/// Gradient X
	Sobel( srcImageGray, grad_x, CV_32FC1, 1, 0, 3);
	/// Gradient Y
	Sobel( srcImageGray, grad_y, CV_32FC1, 0, 1, 3);
	//convertScaleAbs( grad_x, abs_grad_x );
	//convertScaleAbs( grad_y, abs_grad_y );
	//magnitude = abs_grad_x.clone();
	//angle = abs_grad_x.clone();
	//cartToPolar(abs_grad_x , abs_grad_y, magnitude, angle, true);
	cartToPolar(grad_x , grad_y, magnitude, angle, true);
	float numberOfAngleBins = 12;
	int numBins = floor(180/numberOfAngleBins);

	for (int i=0;i<numBins; i++)
		sobelFeatures.push_back(0);

	for (int xIndx=0;xIndx<magnitude.cols; xIndx++)
		for (int yIndx=0;yIndx<magnitude.rows; yIndx++){
			float angleValue = angle.at<float>(xIndx, yIndx);
			if (angleValue > 180)
				angleValue = angleValue - 180;

			if (angleValue != 0)
				angleValue--;

			int currentBin = floor(angleValue / numberOfAngleBins);
			float mag = magnitude.at<float>(xIndx, yIndx);

			//if (angleValue >= 40 && angleValue < 60) // < 0 happens when the angleValue is not a number
				DebugManager::printfToLog(true, "Features::getSobelFeatures", "\nCheck: [%d, %d, angle=%03.05f, mag=%03.05f, bin=%d",
						xIndx, yIndx, angleValue, mag, currentBin);

			if (isnan(mag) || mag < 0)
				mag = 0;

			if (currentBin >= numBins || currentBin < 0) // < 0 happens when the angleValue is not a number
				DebugManager::printfToLog(true, "Features::getSobelFeatures", "\nError: [%d, %d, angle=%03.05f, mag=%03.05f, bin=%d",
						xIndx, yIndx, angleValue, mag, currentBin);
			else
				sobelFeatures[currentBin] = sobelFeatures[currentBin] + mag;
		}
	DebugManager::printfToLog(true, "Features::getSobelFeatures", "\n sobel feat: ");
	for (int i=0;i<numBins; i++)
		DebugManager::printfToLog(true, "Features::getSobelFeatures", "[%d %03.0f, %f] ", i, i*numberOfAngleBins, sobelFeatures[i]);
	return;
}

void Features::splitImageCentroid(const Mat& grey_image, std::vector<Mat>& splitImage){
	float grayValueX = 0;
	float grayValueY = 0;
	float totalGray = 0;
	for (int xIndx=0;xIndx<grey_image.cols; xIndx++)
			for (int yIndx=0;yIndx<grey_image.rows; yIndx++){
				float currValue = grey_image.at<uchar>(xIndx, yIndx);
				DebugManager::printfToLog(true, "Features3::splitImageCentroid2",
						"\n [%d, %d, currValue=%03.05f]",
										xIndx, yIndx, currValue);
				grayValueX += currValue * xIndx;
				grayValueY += currValue * yIndx;
				totalGray += currValue;
			}

	int xMass = floor(grayValueX / totalGray);
	int yMass = floor(grayValueY / totalGray);

	DebugManager::printfToLog(true, "Features::splitImageCentroid",
			"\n [xMass=%03d yMass=%03d], grey_image.cols=%d, grey_image.rows=%d ",
			xMass, yMass, grey_image.cols, grey_image.rows);

	if (xMass > grey_image.cols-1)
		xMass = grey_image.cols-1;
	if (yMass > grey_image.rows-1)
		yMass = grey_image.rows-1;

	if (!DebugManager::noDebugImage("Features::splitImageCentroid")){
		Mat srcImageGray = grey_image.clone();
		line(srcImageGray, Point(xMass, 0), Point(xMass,grey_image.rows-1), 128, 2);
		line(srcImageGray, Point(0, yMass), Point(grey_image.cols-1, yMass), 178, 2);
		char outPath[1024];
		sprintf(outPath, "%s/subImage_ALL.tiff", DebugManager::getDebugDirectoryPath());
		imwrite(outPath, srcImageGray);
	}

	Mat subImage1(grey_image, cv::Rect(0,     0,     xMass,                   yMass));
	Mat subImage2(grey_image, cv::Rect(xMass, 0,     grey_image.cols-1-xMass, yMass));
	Mat subImage3(grey_image, cv::Rect(0,     yMass, xMass,                   grey_image.rows-1-yMass));
	Mat subImage4(grey_image, cv::Rect(xMass, yMass, grey_image.cols-1-xMass, grey_image.rows-1-yMass));
	splitImage.clear();
	splitImage.push_back(subImage1);
	splitImage.push_back(subImage2);
	splitImage.push_back(subImage3);
	splitImage.push_back(subImage4);
}
