#include <opencv2/opencv.hpp>
#include "PutImageToBackground.h"
void PutImageToBackgroundTest()
{
	cv::Mat Src = loadImage("C:\\Users\\Ronald\\Desktop\\A\\templ1.bmp");
	cv::imshow("Src", Src);
	cv::waitKey(1);

	const cv::Size BGSize = cv::Size(200, 200);
	const unsigned char FillValue = 255;
	const cv::Point SrcCenterLocInBG = cv::Point(BGSize.width / 2, BGSize.height / 2);
	cv::Mat Dst;
	int status = PutImageToBackground(Src,
		BGSize,
		FillValue,
		SrcCenterLocInBG,
		Dst);
	cv::imshow("Dst", Dst);
	cv::waitKey(0);
	return;
}