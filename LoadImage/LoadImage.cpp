#include <boost/filesystem.hpp>
#include "LoadImage.h"


#define ERROR (-1)
#define OK (0)

cv::Mat loadImage(const std::string& name)
{
	cv::Mat image = loadImage(name, CV_LOAD_IMAGE_GRAYSCALE);
	return image;
}

cv::Mat loadImage(const std::string& name, int mode)
{
	cv::Mat image = cv::imread(name, mode);
	;
	if (image.empty())
	{
		std::cerr << "Can't load image - " << name << std::endl;
		exit(-1);
	}
#if DEBUG
	cv::imshow(name, image);
	cv::waitKey(1);
#endif
	return image;
}

int loadImage(const std::string& name, int mode, cv::Mat &image)
{
	image = cv::imread(name, mode);

	if (image.empty())
	{
		std::cerr << "Can't load image - " << name << std::endl;
		return ERROR;
	}
#if DEBUG
	cv::imshow(name, image);
	cv::waitKey(1);
#endif
	return OK;
}


// return the filenames of all files that have the specified extension
// in the specified directory and all subdirectories
void get_all(const ::boost::filesystem::path& root,
	const std::string& ext,
	std::vector<::boost::filesystem::path>& ret)
{
	if (!::boost::filesystem::exists(root) || !::boost::filesystem::is_directory(root)) return;

	::boost::filesystem::recursive_directory_iterator it(root);
	::boost::filesystem::recursive_directory_iterator endit;

	while (it != endit)
	{
		if (::boost::filesystem::is_regular_file(*it) && it->path().extension() == ext)
		{
			ret.push_back(it->path());
		}
		++it;
	}
}

int loadAllImagesFromDir(const std::string& dirPath, std::vector<cv::Mat> &images)
{
	std::vector<::boost::filesystem::path> ImgPaths;

	{
		std::vector<::boost::filesystem::path> ImgPathsPerExt;

		ImgPathsPerExt.clear();
		get_all(dirPath,
			".png",
			ImgPaths);
		ImgPaths.insert(ImgPaths.end(), ImgPathsPerExt.begin(), ImgPathsPerExt.end());

		ImgPathsPerExt.clear();
		get_all(dirPath,
			".jpg",
			ImgPaths);
		ImgPaths.insert(ImgPaths.end(), ImgPathsPerExt.begin(), ImgPathsPerExt.end());

		ImgPathsPerExt.clear();
		get_all(dirPath,
			".bmp",
			ImgPaths);
		ImgPaths.insert(ImgPaths.end(), ImgPathsPerExt.begin(), ImgPathsPerExt.end());

	}
	for (int i = 0; i < ImgPaths.size(); ++i)
	{
		cv::Mat img;
		std::string imgPath = ImgPaths.at(i).string();
		loadImage(imgPath, CV_LOAD_IMAGE_GRAYSCALE, img);
		images.push_back(img);
	}
	
	return OK;
}




