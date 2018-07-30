#include "Mask.h"

cv::Rect GetMaskEnclRect(const cv::Mat &Mask)
{
	cv::Mat Points;
	cv::findNonZero(Mask, Points);
	return boundingRect(Points);
}

