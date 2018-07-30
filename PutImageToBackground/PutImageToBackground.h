#pragma once

#include <opencv2/opencv.hpp>

/*********************
Compute a synthetic image with src center at the location of SrcCenterLocInBG.
Background is filled with value FillValue.
Background size is in BGSize.
Return function status Error or OK 
***********************/
int PutImageToBackground(const cv::Mat &Src,
						const cv::Size &BGSize,
						const unsigned char &FillValue,
						const cv::Point &SrcCenterLocInBG,
						cv::Mat &Dst);
