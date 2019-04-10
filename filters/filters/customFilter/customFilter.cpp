// customFilter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "../../../ImageIO/imageFileIO.h"


int hanningFilter()
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


cv::Mat getMean(const std::vector<cv::Mat>& images)
{
	if (images.empty())
	{
		return cv::Mat();
	}

	// Create a 0 initialized image to use as accumulator
	cv::Mat m(images[0].rows, images[0].cols, CV_64F);
	m.setTo(cv::Scalar(0));

	// Use a temp image to hold the conversion of each input image to CV_64FC3
	// This will be allocated just the first time, since all your images have
	// the same size.
	cv::Mat temp;
	for (int i = 0; i < images.size(); ++i)
	{
		// Convert the input images to CV_64FC3 ...
		images[i].convertTo(temp, CV_64F);

		// ... so you can accumulate
		m += temp;
	}

	// Convert back to CV_8UC1 type, applying the division to get the actual mean
	m.convertTo(m, CV_8U, 1. / images.size());
	return m;
}

int main()
{
	std::vector<cv::Mat> mats;
	mats.push_back(cv::imread("inputImage0.bmp", CV_LOAD_IMAGE_GRAYSCALE));
	mats.push_back(cv::imread("inputImage1.bmp", CV_LOAD_IMAGE_GRAYSCALE));
	mats.push_back(cv::imread("inputImage2.bmp", CV_LOAD_IMAGE_GRAYSCALE));
	mats.push_back(cv::imread("inputImage3.bmp", CV_LOAD_IMAGE_GRAYSCALE));
	mats.push_back(cv::imread("inputImage4.bmp", CV_LOAD_IMAGE_GRAYSCALE));
	mats.push_back(cv::imread("inputImage5.bmp", CV_LOAD_IMAGE_GRAYSCALE));
	cv::Mat avgMat;
	avgMat = getMean(mats);
	cv::imwrite("avgMat.bmp", avgMat);
}

