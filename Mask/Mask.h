#pragma once

#include <opencv2/opencv.hpp>

/******************************************
Get Enclosing Rectangle of Valid region (>0)
*******************************************/
cv::Rect GetMaskEnclRect(const cv::Mat &Mask);
