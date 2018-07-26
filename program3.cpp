// program3.cpp
// In this program feature detection and matching is performed using the opencv 
//		features 2D module
// Author: Ambika Hegde
// Course: Computer Vision Spring 2018


#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <iostream>

using namespace cv;

// main - Reads two large images 
//		- Detects keypoints and computes descriptors using brisk detector
//		- Draws keypoints on images
//		- Using brute force matcher, performs matching of two images
//		- Draws matches between two iamges		  
// precondition: two input images exist
// postconditions: Images with keypoints and matched images are displayed
//				   Resulting matched image is written to "output.jpg" 
int main(int argc, char* argv[])
{	
	// Read the input images
	Mat kit1 = imread("kittens1.jpg");
	Mat kit2 = imread("kittens2.jpg");
	Mat desc[2]; // Descriptors
	Mat outImg1, outImg2;

	std::vector<KeyPoint> kpts1, kpts2;
	Ptr<BRISK> detector = BRISK::create();

	// Detect keypoints and compute descriptors 
	detector->detectAndCompute(kit1, Mat(), kpts1, desc[0]);	
	detector->detectAndCompute(kit2, Mat(), kpts2, desc[1]);
	
	// If the images are large, display in a normal window
	namedWindow("keypoints1", WINDOW_NORMAL);
	drawKeypoints(kit1, kpts1, outImg1);
	imshow("keypoints1", outImg1);
	waitKey(0);

	namedWindow("keypoints2", WINDOW_NORMAL);
	drawKeypoints(kit2, kpts2, outImg2);
	imshow("keypoints2", outImg2);
	waitKey(0);

	// Match the descriptors between two images using Brute Force matcher
	Ptr<BFMatcher> matcher = BFMatcher::create(NORM_HAMMING, true);
	std::vector<DMatch> matches;

	try {
		matcher->match(desc[0], desc[1], matches);
	}	
	catch (std::exception& e)
	{	
		std::cout << e.what() << '\n';
		return 0;
	}	
	
	// drawing the results
	Mat matchedImg;
	namedWindow("matches", WINDOW_NORMAL);	
	drawMatches(outImg1, kpts1, outImg2, kpts2, matches, matchedImg);
	imshow("matches", matchedImg);
	imwrite("output.jpg", matchedImg);
	waitKey(0);

	return 0;
}