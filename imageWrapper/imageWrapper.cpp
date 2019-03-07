#include "imageWrapper.h"
static const int null = 0;
// constructor


ImageWrapper::ImageWrapper(int width, int height)
{
	m_imageSize = width * height;
	m_imageHeight = height;
	m_imageWidth = width;
	m_p = new unsigned char[m_imageSize];
}

// destructor
ImageWrapper::~ImageWrapper()
{
	if (m_p)
	{
		delete[] m_p;
	}
	m_imageSize = 0;
	m_imageHeight = 0;
	m_imageWidth = 0;
	m_p = null;
}
// copy constructor
ImageWrapper::ImageWrapper(const ImageWrapper& other)
{
	m_imageSize = other.m_imageSize;
	m_imageHeight = other.m_imageHeight;
	m_imageWidth = other.m_imageWidth;
	m_p = new unsigned char[m_imageSize];
	for (int i = 0; i < m_imageSize; ++i)
	{
		m_p[i] = other.m_p[i];
	}
}
//move constructor
ImageWrapper::ImageWrapper(ImageWrapper&& other)
{
	m_imageSize = other.m_imageSize;
	m_imageHeight = other.m_imageHeight;
	m_imageWidth = other.m_imageWidth;
	m_p = other.m_p;

	other.m_p = null;
	other.m_imageSize = 0;
}

//assignment operator
ImageWrapper& ImageWrapper::operator=(const ImageWrapper& other)
{
	if (&other != this)
	{
		m_imageSize = 0;
		delete [] m_p;
		m_p = null;

		m_imageSize = other.m_imageSize;
		m_imageHeight = other.m_imageHeight;
		m_imageWidth = other.m_imageWidth;
		m_p = new unsigned char[m_imageSize];
		for (int i = 0; i < m_imageSize; ++i)
		{
			m_p[i] = other.m_p[i];
		}
	}
	return *this;
}

//move assignment operator
ImageWrapper& ImageWrapper::operator=(ImageWrapper&& other)
{
	if (&other != this)
	{
		m_imageSize = 0;
		delete[] m_p;

		m_imageSize = other.m_imageSize;
		m_imageHeight = other.m_imageHeight;
		m_imageWidth = other.m_imageWidth;
		m_p = other.m_p;

		other.m_p = null;
		other.m_imageSize = 0;
	}
	return *this;
}

const int ImageWrapper::Height() const
{
	return m_imageHeight;
}

const int ImageWrapper::Width() const
{
	return m_imageWidth;
}
const int ImageWrapper::Size() const
{
	return m_imageSize;
}
unsigned char * ImageWrapper::getData() const
{
	return m_p;
}
unsigned char * ImageWrapper::data()
{
	return m_p;
}

#ifdef USE_OPENCV
int CV8UToImageWrapper(const cv::Mat &mat, ImageWrapper &img)
{
	if (mat.empty())
	{
		std::cout << "conversion fail. image empty.\n";
		return -1;
	}
	if (mat.size().height != img.Height() || mat.size().width != img.Width())
	{
		std::cout << "conversion fail. src tar size inconsistent.\n";
		return -1;
	}

	for (int h = 0; h < img.Height(); ++h)
	{
		for (int w = 0; w < img.Width(); ++w)
		{
			*(img.data() + h * img.Width() + w) = mat.at<unsigned char>(h, w);
		}
	}
	return 0;
}

int ImageWrapperToCV8U(const ImageWrapper &img, cv::Mat &mat)
{
	mat = cv::Mat(img.Height(), img.Width(), CV_8UC1);
	for (int h = 0; h < img.Height(); ++h)
	{
		for (int w = 0; w < img.Width(); ++w)
		{
			mat.at<unsigned char>(h, w) = *(img.getData() + h * img.Width() + w);
		}
	}
	return 0;
}

#endif