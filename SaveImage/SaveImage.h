#pragma once


#include <opencv2/opencv.hpp>
#include <string>

/*********************************
Save Real Image 
**************************************/
int FILE_SaveImageTiffR(cv::Mat matImage,std::string strFilename);

/**********************************
Convert positive real image to 
8 bit(0-255) range and save to strFilename
**************************************/
int FileSaveImage_PveRealto8bit(cv::Mat matImage, std::string strFilename);

/*********************************
Convert real image to
8 bit(0-255) range and save to strFilename
**************************************/
int FileSaveImage_Realto8bit(cv::Mat matImage, std::string strFilename);

