// converter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>

#include <iostream>
#include <algorithm>


#include "../../exrimageFileIO.h"
#include "../../../ImageIO/imageFileIO.h"


int main()
{

	
	cv::Mat m;
	int status = exrimageFileIO::FILE_LoadImageEXR(m, "F:\\ronaldwork\\testData\\images\\v_depth_000000_0001.exr");
	imageFileIO::FILE_SaveImageTiffR(m, "F:\\ronaldwork\\testData\\images\\a.tif");
    return 0;
}

