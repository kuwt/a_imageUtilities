// ImageWrapperTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../imageWrapper.h"
#include "../../imageWrapperT.h"
#include <opencv2\opencv.hpp>
int main()
{
	cv::Mat mat = cv::imread("testcase//img.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	
	/******* imageWrapper test ************/
	{
		ImageWrapper img(mat.size().width, mat.size().height);
		CV8UToImageWrapper(mat, img);

		ImageWrapper img2 = img;
		ImageWrapper img3(img2);
		std::vector<ImageWrapper> imgs;
		imgs.push_back(img);
		imgs.push_back(img2);
		imgs.push_back(img3);

		cv::Mat mat2;
		ImageWrapperToCV8U(imgs.at(2), mat2);
		cv::imshow("img", mat2);
		cv::waitKey(0);
	}
	/******* imageWrapperT test ************/
	{
		ImageWrapperT<unsigned char> img(mat.size().width, mat.size().height);
		CVToImageWrapperT(mat, img);

		ImageWrapperT<unsigned char> img2 = img;
		ImageWrapperT<unsigned char> img3(img2);
		std::vector<ImageWrapperT<unsigned char>> imgs;
		imgs.push_back(img);
		imgs.push_back(img2);
		imgs.push_back(img3);

		cv::Mat mat2;
		ImageWrapperTToCV(imgs.at(2), mat2);
		cv::imshow("img2", mat2);
		cv::waitKey(0);
	}
    return 0;
}

