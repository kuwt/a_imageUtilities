
#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "..\..\..\ImageIO\imageFileIO.h"

void mapDepthValueToColorValue(
	const float &depthValue,
	const float &minDepth,
	const float &maxDepth,
	uchar &R, uchar&G, uchar&B)
{
	if (depthValue < minDepth || depthValue > maxDepth)
	{
		R = 0;
		G = 0;
		B = 0;
		return;
	}
	float tempDepth = 255 * (depthValue - minDepth) / (maxDepth - minDepth);
	float depthRed = 0;
	float depthGreen = 0;
	float depthBlue = 0;

	if (tempDepth < 43) 
	{
		depthRed = tempDepth * 6;
		depthGreen = 0;
		depthBlue = tempDepth * 6;
	}
	if (tempDepth >= 43 && tempDepth < 85)
	{
		depthRed = 255 - (tempDepth - 43) * 6;
		depthGreen = 0;
		depthBlue = 255;
	}
	if (tempDepth >= 85 && tempDepth < 128) 
	{
		depthRed = 0;
		depthGreen = (tempDepth - 85) * 6;
		depthBlue = 255;
	}
	if (tempDepth >= 128 && tempDepth < 169)
	{
		depthRed = 0;
		depthGreen = 255;
		depthBlue = 255 - (tempDepth - 128) * 6;
	}
	if (tempDepth >= 169 && tempDepth < 212) 
	{
		depthRed = (tempDepth - 169) * 6;
		depthGreen = 255;
		depthBlue = 0;
	}
	if (tempDepth >= 212 && tempDepth < 254) 
	{
		depthRed = 255;
		depthGreen = 255 - (tempDepth - 212) * 6;
		depthBlue = 0;
	}
	if (tempDepth >= 254) 
	{
		depthRed = 255;
		depthGreen = 0;
		depthBlue = 0;
	}

	R = depthRed;
	G = depthGreen;
	B = depthBlue;
	return;
}
int main(int argc, char *argv[])
{
	std::string inPath = "depth_0.tif";
	float minDepth = 0;
	float maxDepth = 2000;
	std::string outPath = "colorMap.bmp";

	// handling para
	if (argc < 4)
	{
		// default para
		std::cout << "using default\n";
		std::cout << "inPath = " << inPath << "\n";
		std::cout << "minDepth = " << minDepth << "\n";
		std::cout << "maxDepth = " << maxDepth << "\n";
		std::cout << "outPath = " << outPath << "\n";
	}
	else
	{
		inPath = std::string(argv[1]).c_str();
		minDepth = std::stoi(argv[2]);
		maxDepth = std::stoi(argv[3]);
		outPath = std::string(argv[4]).c_str();

		std::cout << "inPath = " << inPath << "\n";
		std::cout << "minDepth = " << minDepth << "\n";
		std::cout << "maxDepth = " << maxDepth << "\n";
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

	cv::Size imgSize = inImg.size();
	cv::Mat outImg = cv::Mat(imgSize, CV_8UC3);

	for (int h = 0; h < imgSize.height; h++)
	{
		for (int w = 0; w < imgSize.width; w++)
		{
			if (h == 1018 && w == 900)
			{
				std::cout << h << w << "\n";
			}
			float depth = inImg.at<float>(h, w);
			uchar R, G, B;
			mapDepthValueToColorValue(depth, minDepth, maxDepth, B, G, R);
			outImg.at<cv::Vec3b>(h, w) = cv::Vec3b(R,G,B);
		}
	}

	cv::imwrite(outPath, outImg);
	return 0;
}

