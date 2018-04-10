/*
 * MainProcess.cpp
 *
 *  Created on: 31-Oct-2015
 *      Author: kompalli
 */
#include "MainProcess.h"

using namespace std;
using namespace cv;

void MainProcess::printUsage(){
	cout << "\n--------------\nChoose an option: \n\n";
	cout << "testGabor: \t Extract gabor features from image:  <pathToDebugDirectory> <pathToImage> \n";
	cout << "gaborBatch: \t Extract gabor features from image: <pathToDebugDirectory> <pathToImageList> <pathToFeatureFile> \n";
	cout << "testSobel: \t Extract Sobel features from image:  <pathToDebugDirectory> <pathToImage>\n";
	cout << "extractSobelBatch: \t Extract Sobel features from image:  <pathToDebugDirectory> <pathToImageList> <appendPathToImageList> <pathToFeatureFile>\n";
}

int main(int argc, char** argv) {
        if (argc < 2) {
        	MainProcess::printUsage();
                return -1;
        }
        cout << " Option selected: " << argv[1] << "\n\n\n";

        if (strcmp(argv[1], "testGabor") == 0) {
			DebugManager::setDebugPath(argv[2]);
			DebugManager::resetLogFile();

			Features::testGaborFeatures(argv[3]);
			return 0;
        }

        if (strcmp(argv[1], "testSubImage") == 0) {
			DebugManager::setDebugPath(argv[2]);
			DebugManager::resetLogFile();
			DebugManager::printfToLog(true, "main",
					"\nOpening image: %s", argv[3]);
			std::vector<Mat> subImage;
			Mat gray = imread(argv[3], CV_LOAD_IMAGE_GRAYSCALE);
			Size size(40, 40);//the dst image size,e.g.100x100
			resize(gray,gray,size);

			Features::splitImageCentroid(gray, subImage);
			char outPath[1024];
			for (int i=0; i<subImage.size(); i++){
				sprintf(outPath, "%s/subImage_%03d.tiff", DebugManager::getDebugDirectoryPath(), i);
				imwrite(outPath, subImage.at(i));
			}
			return 0;
		}


        if (strcmp(argv[1], "testSobel") == 0) {
			DebugManager::setDebugPath(argv[2]);
			DebugManager::resetLogFile();
			DebugManager::printfToLog(true, "Features::getSobelFeatures",
					"\nOpening image: %s", argv[3]);
			std::vector<float> sobelFeat;
			Mat gray = imread(argv[3], CV_LOAD_IMAGE_GRAYSCALE);
			Features::getSobelFeatures(gray, sobelFeat);
			return 0;
		}

        if (strcmp(argv[1], "extractSobelBatch") == 0) {
			DebugManager::setDebugPath(argv[2]);
			DebugManager::resetLogFile();
			if (argc == 6)
				MainProcess::extractSobelBatch(argv[3], argv[4], argv[5]);
			else
				MainProcess::extractSobelBatch(argv[3], "", argv[4]);
			return 0;
		}
}

int MainProcess::extractSobelBatch(const char* inpImageFileList, const char* imageDirectory, const char* featuresFile){
	bool split = true;
	Size size(40, 40);//the dst image size,e.g.100x100

	FILE *img_list_file;
	if ( (img_list_file=fopen(inpImageFileList,"r"))==NULL ) {
		fprintf(stderr,"Cannot open %s \n", inpImageFileList);
		exit(0);
	}

	char cBuf[1024] = "";
	int fileCount = 0;
	DebugManager::printfToLog(true, "MainProcess::extractSobelBatch",
			"\n\n-----------------\nProcessing file: %s\n", inpImageFileList);

	FILE *grad_fea_output = fopen(featuresFile, "w");

	if (grad_fea_output == NULL){
		fprintf(stderr, "Cannot open feature files for %s\n", inpImageFileList);
		return(0);
	}

	FILE *fptr;
	Mat img;
	std::vector<float> sobelFeat;
	std::vector<Mat> subImage;

	char fullImageName[2048] = "";
	while(fgets(cBuf, sizeof(cBuf), img_list_file) != NULL){
		cBuf[strlen(cBuf) - 1] = '\0';
		fileCount++;
		sprintf(fullImageName, "%s%s", imageDirectory, cBuf);
		if (fileCount % 50 == 0){
			DebugManager::printfToLog(true, "MainProcess::extractSobelBatch",
						"\nProcessing file: %s, time: %s\n", fullImageName, DebugManager::getTime());
			//break;
		}
		if ( (fptr = fopen(fullImageName, "r")) == NULL){
			fprintf(stderr, "Cannot open file#%d: %s\n", fileCount, fullImageName);
			continue;
		}
		fclose(fptr);

		img = imread(fullImageName, CV_LOAD_IMAGE_GRAYSCALE);
		if (split){
			resize(img, img,size);
			Features::splitImageCentroid(img, subImage);
			vector<float> singleSobelFeat;
			sobelFeat.clear();
			for (int i=0; i<subImage.size(); i++){
				singleSobelFeat.clear();
				Features::getSobelFeatures(subImage[i], singleSobelFeat);
				for (int indx=0; indx<singleSobelFeat.size(); indx++)
					sobelFeat.push_back(singleSobelFeat.at(i));
			}
		}else{
			sobelFeat.clear();
			Features::getSobelFeatures(img, sobelFeat);
		}
		for (int i=0;i<sobelFeat.size()-1;i++)
			fprintf(grad_fea_output, "%f, ", sobelFeat[i]);
		fprintf(grad_fea_output, "%f\n", sobelFeat[sobelFeat.size()-1]);

		fflush(grad_fea_output);
	}
	fclose(img_list_file);
	fclose(grad_fea_output);
	return 1;
}
