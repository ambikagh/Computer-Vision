/* Program1.cpp
// 1. Program illustrates the use of OpenCV library functions 
// flip (flips the image horizontally or vertically)
// cvtColor (converts color image to Gray scale image,
// GaussianBlur (Blurs the image)
// Canny (Detects the edges in an image and displays)

// 2. create a window with a track bar to control the level of smoothing (sigmaX and sigmaY) in the image.  
The input should be the grey-level image from above. Use 10 for the maximum sigma. 
Draw the edges to the window each time the track bar is moved. See the example code on the web site. 
This tutorial is also useful (however, I strongly oppose the use of global variables): 
https://docs.opencv.org/master/da/d6a/tutorial_trackbar.html 
// Author: Ambika Hegde
*/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <exception>
#include <iostream>
using namespace cv;


// trackbar_edge - Method performs edge detection every time standard deviation is  
//				   changed using trackbar
// precondition  - Grayscale image has been created
// postcondition - The named window has been updated according to the new settings.
void trackbar_edge(int sigmax, void *src)
{
	Mat in = *(Mat *)src;
	Mat smooth;
	Mat edge;
		
	try
	{
		GaussianBlur(in, smooth, Size(0, 0), sigmax, sigmax);		
	}
	catch (std::exception& e)
	{	// Display the previous image when sigmax and sigmay are zero
		std::cout << e.what() << '\n';
		return;
	}
	Canny(smooth, edge, 20, 60);
	imshow("Edge Detection", edge);
}


// trackbar_brighten - method brightens the image when beta is changed using trackbar
// precondition - Source image "oji.jpg exists"
// postcondition - Brightness of image varies with variation in beta value.
void trackbar_brighten(int beta, void *src)
{
	Mat in = *(Mat *)src;
	// Create a new image with same size and type as source image
	// Set all pixel value to zero
	Mat newImage = Mat::zeros(in.size(), in.type());

	/// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	int alpha = 1;
	for (int y = 0; y < in.rows; y++)
	{
		for (int x = 0; x < in.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				newImage.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(alpha*(in.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}

	imshow("Oji Image", newImage);
}


// main - a quick test of OpenCV functions
// precondition: dozer.jpg exists in the program directory and is a valid JPG
// postconditions: dozer.jpg and images applied with methods flip, cvtColor,
//    GaussianBlur and Canny are displayed

int main(int argc, char* argv[])
{
	Mat image = imread("dozer.jpg");
	namedWindow("Original Image");
	imshow("Original Image", image);
	waitKey(0);

	// Flip the image on x axis
	Mat hFlippedImage; 
	flip(image, hFlippedImage, 1); // flipcode > 0 --> horizontal flipping
	namedWindow("Horizontally Flipped Image");
	imshow("Horizontally Flipped Image", hFlippedImage);
	waitKey(0);

	// Flip the image on y axis
	Mat vFlippedImage; 
	flip(hFlippedImage, vFlippedImage, 0); // flipcode 0 --> vertical flipping
	namedWindow("Vertically Flipped Image");
	imshow("Vertically Flipped Image", vFlippedImage);
	waitKey(0);

	// Flip the image on both x and y axis
	Mat xyFlippedImage;
	flip(image, xyFlippedImage, -1); // flipcode -1 --> horizontal and vertical flipping
	namedWindow("Flipped Image xy");
	imshow("Flipped Image xy", xyFlippedImage);
	waitKey(0);

	// Convert image from color to grayscale
	Mat grayImage;
	cvtColor(xyFlippedImage, grayImage, CV_BGR2GRAY);
	namedWindow("Grayscale Image");
	imshow("Grayscale Image", grayImage);
	waitKey(0);

	
	// Perform blurring of the image using Gaussian filter
	Mat gBlurredImage;
	GaussianBlur(grayImage, gBlurredImage, Size(0, 0), 2, 2);
	namedWindow("Blurred Image");
	imshow("Blurred Image", gBlurredImage);
	waitKey(0);

	// Detect the edges of blurred image
	Mat edgeImage;
	Canny(gBlurredImage, edgeImage, 20, 60);
	namedWindow("Edge Image");
	imshow("Edge Image", edgeImage);
	waitKey(0);

	// write the output to output.jpg
	imwrite("output.jpg", edgeImage);

		
	// create a track bar to control the level of smoothing
	// input is grayImage, max sigma is 10
	int sigmax = -1;
	const int maxSigma = 10;
	namedWindow("Edge Detection");
	createTrackbar("Sd", "Edge Detection", &sigmax, maxSigma, trackbar_edge , &grayImage);	
	waitKey(0);
		

	// Change the brightness of the image
	// Input image is Oji.jpg
	Mat image1 = imread("oji.jpg");
	namedWindow("Oji Image");
	imshow("Oji Image", image1);
	int beta = 10;
	const int maxBeta = 100;
	createTrackbar("beta", "Oji Image", &beta, maxBeta, trackbar_brighten, &image1);
	waitKey(0);

	// Blend two images
	Mat image2 = imread("9.jpg");
	namedWindow("Oji Image2");
	imshow("Oji Image2", image2);
	waitKey(0);
	Mat image3 = imread("10.jpg");
	namedWindow("Oji Image3");
	imshow("Oji Image3", image3);
	waitKey(0);

	double a = 0.5, b;
	Mat blended;
	b = 1 - a;
	addWeighted(image2, a, image3, b, 0.0, blended);
	imshow("Blended Image", blended);
	waitKey(0);
	
	return 0;
}

