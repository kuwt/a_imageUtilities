// medianFilter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\opencv.hpp"

int main(int argc, char *argv[])
{
	std::string inPath;
	int filterSize = 5;
	std::string outPath;

	// handling para
	if (argc < 4)
	{
		// default para
		std::cout << "using default\n";
		std::cout << "inPath = " << inPath << "\n";
		std::cout << "filterSize = " << filterSize << "\n";
		std::cout << "outPath = " << outPath << "\n";
	}
	else
	{
		inPath = std::string(argv[1]).c_str();
		filterSize = std::stoi(argv[2]);
		outPath = std::string(argv[3]).c_str();

		std::cout << "inPath = " << inPath << "\n";
		std::cout << "filterSize = " << filterSize << "\n";
		std::cout << "outPath = " << outPath << "\n";
	}
	
	cv::Mat inImg = cv::imread(inPath, CV_LOAD_IMAGE_GRAYSCALE);
	if (inImg.empty())
	{
		std::cout << "empty image" << "\n";
		getchar();
		return -1;
	}
	if (outPath == "")
	{
		std::cout << "empty outPath" << "\n";
		getchar();
		return -1;
	}

	cv::Mat outImg;
	cv::medianBlur(inImg, outImg, 5);
	cv::imwrite(outPath,outImg);

    return 0;
}

