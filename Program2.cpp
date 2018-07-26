// program2.cpp
// Chroma key: This program identifies the most common color in the foreground image
//		and replaces that color with a background image.
// Author: Ambika Hegde
// Course: Computer Vision Spring 2018

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

// Contains foreground image, background image and most common color information
// Contains information needed in trackbar callback function
struct ImagePlusMostCommon {
	Mat fImage; // foreground image
	Mat bImage; // background image
	int cB;
	int cG;
	int cR;
};

// on_trackbar - Method replaces most common color and nearby color in foreground image with 
//				   corresponding pixel in background image
// precondition  - Foreground and background image exists, common colors Cb, cG, cR 
//				   are calculated
// postcondition - The named window has been updated according to the new settings.
void on_trackbar(int size, void *src)
{
	ImagePlusMostCommon imageInfo = *(ImagePlusMostCommon *)src;
	Mat inForeg = imageInfo.fImage.clone();

	Vec3b p;
	for (int r = 0; r < inForeg.rows; r++) {
		for (int c = 0; c < inForeg.cols; c++) {
			p = inForeg.at<Vec3b>(r, c);
			/*
			int blue = p[0];
			int green = p[1];
			int red = p[2];
			*/
			int absDiffBlue = abs(p[0] - imageInfo.cB);
			int absDiffGreen = abs(p[1]) - imageInfo.cG;
			int absDiffRed = abs(p[2] - imageInfo.cR);

			if (absDiffBlue < size && absDiffGreen < size && absDiffRed < size) {
				inForeg.at<Vec3b>(r, c) = imageInfo.bImage.at<Vec3b>(r % imageInfo.bImage.rows,
					c % imageInfo.bImage.cols);
			}
		}
	}
	imshow("Chromakey Image", inForeg);
	// Store the resulting image
	imwrite("overlay.jpg", inForeg);
}



// main - Reads foreground image and background image
//		- Finds the most common color in foreground image
//		- Replaces every pixel in the foreground image that is close to most common
//			color with the corresponding pixel from the background image		  
// precondition: foreground.jpg and background.jpg exists in the code directory and is a valid JPG
// postconditions: The foreground and background images are displayed on the screen
//					Foreground image with trackbar specifying closeness to common color) is displayed.
//					Resulting image is written to "overlay.jpg" 
int main(int argc, char* argv[])
{
	Mat foreg = imread("foreground.jpg");
	// Uncomment following line and comment above line to see some interesting effects
	// Mat foreg = imread("foreground1.jpg"); 
	namedWindow("foreground Image");
	imshow("foreground Image", foreg);
	waitKey(0);

	Mat backg = imread("background.jpg");
	// Uncomment following line and comment above line to see some interesting effects
	// Mat backg = imread("background1.jpg");
	namedWindow("background Image");
	imshow("background Image", backg);
	waitKey(0);

	// Use color histogram to find the most common color in foreground image
	const int size = 4;
	const int bucketSize = 256 / size;
	int dims[] = { size, size, size };  // the # of buckets in each dimension  
										// 3D histogram of integers initialized to zero
	Mat hist(3, dims, CV_32S, Scalar::all(0));

	// Loop through the foreground image and assign each pixel to a histogram bucket.
	for (int r = 0; r < foreg.rows; r++) {
		for (int c = 0; c < foreg.cols; c++) {
			int blue = foreg.at<Vec3b>(r, c)[0];
			int green = foreg.at<Vec3b>(r, c)[1];
			int red = foreg.at<Vec3b>(r, c)[2];
			int x = blue / bucketSize;
			int y = green / bucketSize;
			int z = red / bucketSize;
			hist.at<int>(x, y, z) += 1;
		}
	}

	// Find the histogram with the most votes
	int max = 0;
	int x_, y_, z_;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int z = 0; z < size; z++) {
				if (hist.at<int>(x, y, z) > max) {
					max = hist.at<int>(x, y, z);
					x_ = x;
					y_ = y;
					z_ = z;
				}
			}
		}
	}

	// Common color calculation
	int cBlue = x_ * bucketSize + bucketSize / 2;
	int cGreen = y_ * bucketSize + bucketSize / 2;
	int cRed = z_ * bucketSize + bucketSize / 2;

	// src holds all required information that needs to be passed to trackbar
	ImagePlusMostCommon src;
	src.fImage = foreg;
	src.bImage = backg;
	src.cB = cBlue;
	src.cG = cGreen;
	src.cR = cRed;

	int minBucketSlider = bucketSize / 2;
	const int maxBucketSlider = bucketSize;
	namedWindow("Chromakey Image");
	imshow("Chromakey Image", foreg);
	createTrackbar("Size", "Chromakey Image", &minBucketSlider, maxBucketSlider, on_trackbar, &src);

	waitKey(0);

	return 0;
}