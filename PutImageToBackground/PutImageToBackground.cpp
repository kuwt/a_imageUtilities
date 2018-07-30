#include "PutImageToBackground.h"

#define ERROR (-1)
#define OK (0)

int PutImageToBackground(const cv::Mat &Src,
						const cv::Size &BGSize,
						const unsigned char &FillValue,
						const cv::Point &SrcCenterLocInBG,
						cv::Mat &Dst)
{
	// Check if BGSize > Src size
	if (BGSize.width < Src.size().width || BGSize.height < Src.size().height)
	{
		return ERROR;
	}
	/***  REDUNDANT CHECK
	// Check if SrcCenterLoc inside BGSize
	if ( SrcCenterLocInBG.x < 0 || SrcCenterLocInBG.x > BGSize.width
		|| SrcCenterLocInBG.y < 0 || SrcCenterLocInBG.y > BGSize.height)
	{
		return ERROR;
	}
	*/

	
	// And Check if SrcCenterLoc + Src Exceed BGSize
	cv::Point ExpectedUL;
	ExpectedUL.x = SrcCenterLocInBG.x - Src.size().width / 2;
	ExpectedUL.y = SrcCenterLocInBG.y - Src.size().height / 2;
	cv::Point ExpectedLR;
	ExpectedLR.x = ExpectedUL.x + Src.size().width;
	ExpectedLR.y = ExpectedUL.y + Src.size().height;

	if (ExpectedUL.x < 0 || ExpectedUL.y < 0 || ExpectedLR.x > BGSize.width || ExpectedLR.y >  BGSize.height)
	{
		return ERROR;
	}

	// init SynMat with fill value
	cv::Mat SynMat = cv::Mat(BGSize, CV_8UC1, cv::Scalar(FillValue));

	// Calculate roi offset and offsize according to SrcCenterLoc amd Src 
	cv::Rect ROI(ExpectedUL, ExpectedLR);
	
	// Copy Src to the ROI of SynMat.
	Src.copyTo(SynMat(ROI));

	// return SynMat
	Dst = SynMat;
	return OK;
}