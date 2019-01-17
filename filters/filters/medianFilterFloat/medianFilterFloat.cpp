

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "..\..\..\ImageIO\imageFileIO.h"
int main(int argc, char *argv[])
{
	std::string inPath = "caliStep_0_phase.tif";
	int filterSize = 5;
	std::string outPath = "out.tif";

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
	cv::Mat inImg;
	int status = imageFileIO::FILE_LoadImageTiffR(inImg, inPath);
	if (status != 0)
	{
		std::cout << "FILE_LoadImageTiffR fail" << "\n";
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
	status = imageFileIO::FILE_SaveImageTiffR(outImg, outPath);
	if (status != 0)
	{
		std::cout << "FILE_SaveImageTiffR fail" << "\n";
		getchar();
		return -1;
	}

	return 0;
}

