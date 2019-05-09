#include "exrimageFileIO.h"


#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <ImfNamespace.h>

#include <iostream>
#include <algorithm>
#include <experimental/filesystem>

using namespace OPENEXR_IMF_NAMESPACE;
using namespace std;
using namespace IMATH_NAMESPACE;

void readRgba1(const char fileName[],
	Array2D<Rgba> &pixels,
	int &width,
	int &height)
{
	RgbaInputFile file(fileName);
	Box2i dw = file.dataWindow();
	width = dw.max.x - dw.min.x + 1;
	height = dw.max.y - dw.min.y + 1;
	pixels.resizeErase(height, width);
	file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
	file.readPixels(dw.min.y, dw.max.y);
}

int exrimageFileIO::FILE_LoadImageEXR(cv::Mat &matImage, std::string strFilename)
{
	if (!std::experimental::filesystem::exists(strFilename))
	{
		std::cout << "path = " << strFilename << " does not exist!\n";
		return -1;
	}


	Array2D<Rgba> pixels;
	int width = 0;
	int height = 0;

	readRgba1(strFilename.c_str(),
		pixels,
		width,
		height);

	cv::Mat m = cv::Mat(cv::Size(width, height), CV_32FC1);
	for (int h = 0; h < height; ++h)
	{
		for (int w = 0; w < width; ++w)
		{
			float value = (float)pixels[h][w].r;
			m.at<float>(h, w) = value;
		}
	}
	matImage = m;
	return 0;
}
