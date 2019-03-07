#pragma once

class ImageWrapper
{
public:
	ImageWrapper(int width, int height);
	virtual ~ImageWrapper();
	ImageWrapper(const ImageWrapper&other);
	ImageWrapper& operator=(const ImageWrapper& other);
	ImageWrapper(ImageWrapper&& other);
	ImageWrapper& operator=(ImageWrapper&& other);
	
	const int Size() const;
	const int Height() const;
	const int Width() const;
	unsigned char * getData() const;
	unsigned char * data();
private:
	int m_imageSize = 0;
	int m_imageHeight = 0;
	int m_imageWidth = 0;
	unsigned char *m_p = 0;
};


#ifdef USE_OPENCV
#include <opencv2\opencv.hpp>
int ImageWrapperToCV8U(const ImageWrapper &img, cv::Mat &mat);
int CV8UToImageWrapper(const cv::Mat &mat, ImageWrapper &img);
#endif