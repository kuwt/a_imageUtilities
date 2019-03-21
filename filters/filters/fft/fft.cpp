// fft.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "..\..\..\ImageIO\imageFileIO.h"
using namespace cv;

void circshift(Mat &out, const Point &delta)
{
	Size sz = out.size();
	assert(sz.height > 0 && sz.width > 0);
	if ((sz.height == 1 && sz.width == 1) || (delta.x == 0 && delta.y == 0))
		return;

	int x = delta.x;
	int y = delta.y;
	if (x > 0) x = x % sz.width;
	if (y > 0) y = y % sz.height;
	if (x < 0) x = x % sz.width + sz.width;
	if (y < 0) y = y % sz.height + sz.height;

	std::vector<cv::Mat> planes;
	cv::split(out, planes);

	for (size_t i = 0; i < planes.size(); i++)
	{
		
		Mat tmp0, tmp1, tmp2, tmp3;
		Mat q0(planes[i], Rect(0, 0, sz.width, sz.height - y));
		Mat q1(planes[i], Rect(0, sz.height - y, sz.width, y));
		q0.copyTo(tmp0);
		q1.copyTo(tmp1);
		tmp0.copyTo(planes[i](Rect(0, y, sz.width, sz.height - y)));
		tmp1.copyTo(planes[i](Rect(0, 0, sz.width, y)));

		Mat q2(planes[i], Rect(0, 0, sz.width - x, sz.height));
		Mat q3(planes[i], Rect(sz.width - x, 0, x, sz.height));
		q2.copyTo(tmp2);
		q3.copyTo(tmp3);
		tmp2.copyTo(planes[i](Rect(x, 0, sz.width - x, sz.height)));
		tmp3.copyTo(planes[i](Rect(0, 0, x, sz.height)));
	}

	merge(planes, out);
}
void fftshift(Mat &out)
{
	Size sz = out.size();
	Point pt(0, 0);
	pt.x = (int)floor(sz.width / 2.0);
	pt.y = (int)floor(sz.height / 2.0);
	circshift(out, pt);
}

void ifftshift(Mat &out)
{
	Size sz = out.size();
	Point pt(0, 0);
	pt.x = (int)ceil(sz.width / 2.0);
	pt.y = (int)ceil(sz.height / 2.0);
	circshift(out, pt);
}


int fft_Test(cv::Mat I)
{
	/**************** padded******************/
	cv::Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols); // on the border add zero values
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, cv::Scalar::all(0));

	/**************** make place******************/
	cv::Mat planes[] = { cv::Mat_<float>(padded),cv::Mat::zeros(padded.size(), CV_32F) };
	cv::Mat complexI;
	cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	/**************** dft******************/
	cv::dft(complexI, complexI);            // this way the result may fit in the source matrix

										// compute the magnitude and switch to logarithmic scale
										// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))

	
	/**************** display******************/
	cv::split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	cv::magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	cv::Mat magI = planes[0];

	magI += cv::Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);

	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
	cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
	cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

	cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
											// viewable image form (float between values 0 and 1).

	imshow("Input Image", I);    // Show the result
	imshow("spectrum magnitude", magI);
	waitKey();

	cv::Mat ifft;
	cv::idft(complexI, ifft, DFT_REAL_OUTPUT);
	normalize(ifft, ifft, 0, 1, CV_MINMAX);
	imshow("idft", ifft);
	waitKey();
	return 0;
}

int phaseWrap_mftp(
	const cv::Mat &ori, 
	const cv::Mat &fringe,
	std::string logPath,
	bool isLog,
	cv::Mat &o)
{
	cv::Mat inv = ori - fringe;
	if (isLog)
	{
		char str[256];
		snprintf(str, 256, "%s\\inv.bmp", logPath);
		cv::imwrite(str, inv);
	}
	cv::Mat fringe32F;
	fringe.convertTo(fringe32F, CV_32F);
	cv::Mat inv32F;
	inv.convertTo(inv32F, CV_32F);
	cv::Mat InoDC32F;
	InoDC32F = fringe32F - inv32F;
	if (isLog)
	{
		char str[256];
		snprintf(str, 256, "%s\\noDC.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(InoDC32F, str);
	}
	
	cv::Mat I = InoDC32F;
	/**************** padded******************/
	cv::Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols); // on the border add zero values
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, cv::Scalar::all(0));

	/**************** make place******************/
	cv::Mat planes[] = { cv::Mat_<float>(padded),cv::Mat::zeros(padded.size(), CV_32F) };
	cv::Mat complexI;
	cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	/**************** dft******************/
	cv::dft(complexI, complexI);            // this way the result may fit in the source matrix

											// compute the magnitude and switch to logarithmic scale
											// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))

	/**************** display******************/
	/*
	{
		cv::Mat planesdisp[2];
		cv::split(complexI, planesdisp);					// planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
		cv::Mat magI;
		cv::magnitude(planesdisp[0], planesdisp[1], magI);	// planes[0] = magnitude

		magI += cv::Scalar::all(1);                    // switch to logarithmic scale
		log(magI, magI);

		// crop the spectrum, if it has an odd number of rows or columns
		magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

		// rearrange the quadrants of Fourier image  so that the origin is at the image center
		int cx = magI.cols / 2;
		int cy = magI.rows / 2;

		cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
		cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
		cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

		cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);

		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);

		normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
												// viewable image form (float between values 0 and 1)
	}
	*/
	/**************** fftshift ******************/
	cv::Mat planes_shift[2];
	cv::split(complexI, planes_shift);
	if (isLog)
	{
		char str[256];
		snprintf(str, 256, "%s\\real.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(planes_shift[0], str);
		snprintf(str, 256, "%s\\imag.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(planes_shift[1], str);
	}

	fftshift(planes_shift[0]);
	fftshift(planes_shift[1]);
	if (isLog)
	{
		char str[256];
		snprintf(str, 256, "%s\\real_shf.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(planes_shift[0], str);
		snprintf(str, 256, "%s\\imag_shf.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(planes_shift[1], str);
	}

	/**************** filter ******************/
	cv::Mat filter = cv::Mat::zeros(planes_shift[0].size(),CV_32F);
	for (int j = 0; j < filter.size().height; ++j)
	{
		for (int i = 0; i < filter.size().width; ++i)
		{
			if (i <= 638)
			{
				filter.at<float>(j,i) = 1;
			}
		}
	}

	cv::Mat f_real = planes_shift[0].mul(filter);
	cv::Mat f_imag = planes_shift[1].mul(filter);
	if (isLog)
	{
		char str[256];
		snprintf(str, 256, "%s\\f_real.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(f_imag, str);
		snprintf(str, 256, "%s\\f_imag.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(f_imag, str);
	}

	/**************** ifftshift ******************/
	ifftshift(f_real);
	ifftshift(f_imag);
	if (isLog)
	{
		char str[256];
		snprintf(str, 256, "%s\\f_real_ishift.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(f_real, str);
		snprintf(str, 256, "%s\\f_imag_ishift.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(f_imag, str);
	}
	cv::Mat planesBack[] = { cv::Mat_<float>(f_real), cv::Mat_<float>(f_imag) };
	cv::Mat complexBack;
	cv::merge(planesBack, 2, complexBack);

	cv::Mat ifft;
	cv::idft(complexBack, ifft, cv::DFT_COMPLEX_OUTPUT);
	cv::Mat ifftsplit[2];
	cv::split(ifft, ifftsplit);

	cv::Mat phase = cv::Mat(ifft.size(), CV_32F);
	for (int j = 0; j < ifft.size().height; ++j)
	{
		for (int i = 0; i < ifft.size().width; ++i)
		{
			float phi = atan2(ifftsplit[1].at<float>(j,i), ifftsplit[0].at<float>(j, i));
			phase.at<float>(j,i) = phi;
		}
	}

	if (isLog)
	{
		char str[256];
		snprintf(str, 256, "%s\\phase.tif", logPath);
		imageFileIO::FILE_SaveImageTiffR(phase, str);
	}
	o = phase;

	return 0;
}

int main(int argc, char *argv[])
{

	cv::Mat I = imread("testcase\\0000.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	if (I.empty())
	{
		return -1;
	}

	cv::Mat II = imread("testcase\\0001.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	if (II.empty())
	{
		return -1;
	}
	
	cv::Mat III = imread("testcase\\0002.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	if (III.empty())
	{
		return -1;
	}

	cv::Mat IV = imread("testcase\\0003.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	if (IV.empty())
	{
		return -1;
	}
	std::string logPath = "log";
	cv::Mat phase;
	phaseWrap_mftp(
		I,
		II,
		logPath,
		false,
		phase);
	imageFileIO::FILE_SaveImageTiffR(phase, "log\\phase1.tif");

	cv::Mat phase2;
	phaseWrap_mftp(
		I,
		III,
		logPath,
		false,
		phase2);
	imageFileIO::FILE_SaveImageTiffR(phase2, "log\\phase2.tif");

	cv::Mat phase3;
	phaseWrap_mftp(
		I,
		IV,
		logPath,
		false,
		phase3);
	imageFileIO::FILE_SaveImageTiffR(phase3, "log\\phase3.tif");
		

	return 0;
}