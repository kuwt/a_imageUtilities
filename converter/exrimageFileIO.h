#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class exrimageFileIO
{
public:
	static const int OK = 0;
	static const int ERR = -1;
	
	/*****************************
	*	Load EXR
	****************************/
	/****  Load float 16bit real Image  ***/
	static int FILE_LoadImageEXR(cv::Mat &matImage, std::string strFilename);
};


