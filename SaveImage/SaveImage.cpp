
#include "SaveImage.h"

#include <tiff.h>
#include <tiffio.h>
#include <iostream>
#include <fstream>
#include <math.h>

#define ok 0
#define	__MAX_PATH	256

int FILE_SaveImageTiffR(cv::Mat matImage,
	std::string strFilename)
{
	int	intStatus = ok;
	CV_Assert(matImage.depth() == CV_32F);

	char* acBuffer;
	acBuffer = (char*)malloc(strFilename.size() + 1);
	memcpy(acBuffer, strFilename.c_str(), strFilename.size() + 1);

	unsigned int line_size = matImage.cols * 4;
	tdata_t row_buf = _TIFFmalloc(line_size);

	// call libtiff
	TIFF* tif = TIFFOpen(acBuffer, "w");
	if (tif == NULL)
	{
		intStatus = -1;
		goto Exit;
	}

	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, matImage.cols);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, matImage.rows);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8 * 4);
	TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, 3);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, 1);
	TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, 2);

	float* src = matImage.ptr<float>(0);



	for (int row = 0; row < matImage.rows; ++row)
	{
		memcpy(row_buf, src, line_size);
		TIFFWriteScanline(tif, row_buf, row);
		src += matImage.cols;
	}

Exit:
	_TIFFfree(row_buf);
	TIFFClose(tif);
	free(acBuffer);
	return intStatus;
}


int FileSaveImage_PveRealto8bit(cv::Mat matImage, std::string strFilename)
{
	int intStatus = ok;
	double dMin;
	double dMax;
	cv::minMaxIdx(matImage, &dMin, &dMax);
	cv::Mat matAdjustImage;
	cv::convertScaleAbs(matImage, matAdjustImage, 255 / dMax);
	intStatus = cv::imwrite(strFilename, matAdjustImage);
	return intStatus;
}

int FileSaveImage_Realto8bit(cv::Mat matImage, std::string strFilename)
{
	int intStatus = ok;
	double dMin;
	double dMax;
	cv::minMaxIdx(matImage, &dMin, &dMax);

	double dScalingFactor = abs(dMin) > abs(dMax) ? abs(dMin) : abs(dMax);

	cv::Mat matAdjustImage;
	matAdjustImage = matImage * (128.0 / dScalingFactor);
	//cv::minMaxIdx(adjimage, &min, &max);
	matAdjustImage = matAdjustImage + 127;
	//cv::minMaxIdx(adjimage, &min, &max);
	cv::Mat matAdjustImage2;
	matAdjustImage.convertTo(matAdjustImage2, CV_8U, 1.0);
	intStatus = imwrite(strFilename, matAdjustImage2);
	return intStatus;
}

