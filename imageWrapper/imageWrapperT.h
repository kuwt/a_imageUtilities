#pragma once
#include <typeinfo> 

template<typename Scalar>
class ImageWrapperT
{
public:
	ImageWrapperT(int width, int height)
	{
		m_imageSize = width * height;
		m_imageHeight = height;
		m_imageWidth = width;
		m_p = new Scalar[m_imageSize];
	}
	virtual ~ImageWrapperT()
	{
		if (m_p)
		{
			delete[] m_p;
		}
		m_imageSize = 0;
		m_imageHeight = 0;
		m_imageWidth = 0;
		m_p = 0;
	}
	ImageWrapperT(const ImageWrapperT&other)
	{
		m_imageSize = other.m_imageSize;
		m_imageHeight = other.m_imageHeight;
		m_imageWidth = other.m_imageWidth;
		m_p = new Scalar[m_imageSize];
		for (int i = 0; i < m_imageSize; ++i)
		{
			m_p[i] = other.m_p[i];
		}
	}
	ImageWrapperT& operator=(const ImageWrapperT& other)
	{
		if (&other != this)
		{
			m_imageSize = 0;
			delete[] m_p;
			m_p = 0;

			m_imageSize = other.m_imageSize;
			m_imageHeight = other.m_imageHeight;
			m_imageWidth = other.m_imageWidth;
			m_p = new Scalar[m_imageSize];
			for (int i = 0; i < m_imageSize; ++i)
			{
				m_p[i] = other.m_p[i];
			}
		}
		return *this;
	}

	ImageWrapperT(ImageWrapperT&& other)
	{
		m_imageSize = other.m_imageSize;
		m_imageHeight = other.m_imageHeight;
		m_imageWidth = other.m_imageWidth;
		m_p = other.m_p;

		other.m_p = 0;
		other.m_imageSize = 0;
	}

	ImageWrapperT& operator=(ImageWrapperT&& other)
	{
		if (&other != this)
		{
			m_imageSize = 0;
			delete[] m_p;

			m_imageSize = other.m_imageSize;
			m_imageHeight = other.m_imageHeight;
			m_imageWidth = other.m_imageWidth;
			m_p = other.m_p;

			other.m_p = 0;
			other.m_imageSize = 0;
		}
		return *this;
	}
	
	const int Size() const
	{
		return m_imageSize;
	}
	const int Height() const
	{
		return m_imageHeight;
	}

	const int Width() const
	{
		return m_imageWidth;
	}
	Scalar * getData() const
	{
		return m_p;
	}
	Scalar * data()
	{
		return m_p;
	}
private:
	int m_imageSize = 0;
	int m_imageHeight = 0;
	int m_imageWidth = 0;
	Scalar *m_p = 0;
};


#ifdef USE_OPENCV
#include <opencv2\opencv.hpp>
template <class Scalar>
int ImageWrapperTToCV(const ImageWrapperT<Scalar> &img, cv::Mat &mat)
{
	if (std::string(typeid(Scalar).name()) == "unsigned char")
	{
		mat = cv::Mat(img.Height(), img.Width(), CV_8UC1);
	}
	else if (std::string(typeid(Scalar).name()) == "float")
	{
		mat = cv::Mat(img.Height(), img.Width(), CV_32FC1);
	}
	else if (std::string(typeid(Scalar).name()) == "int")
	{
		mat = cv::Mat(img.Height(), img.Width(), CV_32SC1);
	}
	else if (std::string(typeid(Scalar).name()) == "double")
	{
		mat = cv::Mat(img.Height(), img.Width(), CV_64FC1);
	}
	else
	{
		std::cout << "not support data format.\n";
		return -1;
	}
	
	for (int h = 0; h < img.Height(); ++h)
	{
		for (int w = 0; w < img.Width(); ++w)
		{
			mat.at<unsigned char>(h, w) = *(img.getData() + h * img.Width() + w);
		}
	}
	return 0;
}
template <class Scalar>
int CVToImageWrapperT(const cv::Mat &mat, ImageWrapperT<Scalar> &img)
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
	// check type
	//std::cout << typeid(Scalar).name() << "\n";
	if (std::string(typeid(Scalar).name()) == "unsigned char" )
	{
		if (mat.type() != CV_8UC1)
		{
			std::cout << "conversion fail. type inconsistent.\n";
			return -1;
		}
	}
	else if (std::string(typeid(Scalar).name()) == "float")
	{
		if (mat.type() != CV_32FC1)
		{
			std::cout << "conversion fail. type inconsistent.\n";
			return -1;
		}
	}
	else if (std::string(typeid(Scalar).name()) == "int")
	{
		if (mat.type() != CV_32SC1)
		{
			std::cout << "conversion fail. type inconsistent.\n";
			return -1;
		}
	}
	else if (std::string(typeid(Scalar).name()) == "double")
	{
		if (mat.type() != CV_64FC1)
		{
			std::cout << "conversion fail. type inconsistent.\n";
			return -1;
		}
	}
	else
	{
		std::cout << "not support data format.\n";
		return -1;
	}



	for (int h = 0; h < img.Height(); ++h)
	{
		for (int w = 0; w < img.Width(); ++w)
		{
			*(img.data() + h * img.Width() + w) = mat.at<Scalar>(h, w);
		}
	}
	return 0;
}
#endif
