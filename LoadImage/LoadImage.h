#pragma once

#include <opencv2/opencv.hpp>

/*************************
Load a single image
Load as CV_LOAD_IMAGE_GRAYSCALE
**************************/
cv::Mat loadImage(const std::string& name);

/*************************
Load a single image
User specify the mode, e.g. CV_LOAD_IMAGE_GRAYSCALE
*************************/
cv::Mat loadImage(const std::string& name, int mode);

/*************************
Load a single image
 User specify the mode, e.g. CV_LOAD_IMAGE_GRAYSCALE
return status
*************************/
int loadImage(const std::string& name, int mode, cv::Mat &image);

/*************************
Load all images in a directory
User specify the mode, e.g. CV_LOAD_IMAGE_GRAYSCALE
return status
*************************/
int loadAllImagesFromDir(const std::string& dirPath, std::vector<cv::Mat> &images);