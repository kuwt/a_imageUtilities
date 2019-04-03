// customFilter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "../../../ImageIO/imageFileIO.h"

int main()
{
	bool isLog = true;
	std::string logPath = ".";

	cv::Mat largehann = cv::Mat::zeros(cv::Size(1280, 720), CV_32FC1);
	{
		cv::Mat hann;
		cv::Size hannSize = cv::Size(25, 25);
		cv::createHanningWindow(hann, hannSize, CV_32FC1);
		cv::Point hannCenter = cv::Point(681, 361);
		cv::Point hannTL = cv::Point(hannCenter.x - hannSize.width / 2, hannCenter.y - hannSize.height / 2);
		cv::Rect roi = cv::Rect(hannTL, hannSize);
		hann.copyTo(largehann(roi));
	}

	if (isLog)
	{
		char str[256];
		snprintf(str, 256, "%s\\hann.tif", logPath.c_str());
		imageFileIO::FILE_SaveImageTiffR(largehann, str);
	}

    return 0;
}

