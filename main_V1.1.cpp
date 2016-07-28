#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Rect R;

Mat findLargestContour(Mat thr) {

	vector< vector <Point> > contours; // Vector for storing contour
	vector< Vec4i > hierarchy;
	int largest_contour_index = 0;
	int largest_area = 0;
	Mat dst(thr.rows, thr.cols, CV_8UC1, Scalar::all(0)); //create destination image

	findContours(thr, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i< contours.size(); i++) // iterate through each contour.
	{
		double a = contourArea(contours[i], false);  //  Find the area of contour
		if (a>largest_area) {
			largest_area = a;
			largest_contour_index = i;                //Store the index of largest contour
		}
	}
	drawContours(dst, contours, largest_contour_index, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
	R = boundingRect(contours[largest_contour_index]);
	return dst;

}

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);

	// Load cascate classifier placed in sulution folder
	CascadeClassifier detector;
	string cascadeName = "haarcascade_frontalface_alt.xml";
	bool loaded = detector.load(cascadeName);

	// Parameters of detectMultiscale Cascade Classifier

	int groundThreshold = 2;
	double scaleStep = 1.1;
	Size minimalObjectSize(80, 80);
	Size maximalObjectSize(200, 200);


	// Vector of returned faces
	vector<Rect> found;


	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);

		// Detect faces
		detector.detectMultiScale(edges, found, scaleStep, groundThreshold, 0 | 2, minimalObjectSize, maximalObjectSize);

		// Draw the results into mat retrieved from webcam
		if (found.size() > 0) {
			for (int i = 0; i <= found.size() - 1; i++) {
				rectangle(edges, found[i].br(), found[i].tl(), Scalar(0, 0, 0), 1, 8, 0);
				rectangle(frame, found[i].br(), found[i].tl(), Scalar(0, 0, 0), 1, 8, 0);
			}
		}

		Canny(edges, edges, 30, 255);

		Size kernalSize(5, 5);
		Mat element = getStructuringElement(MORPH_RECT, kernalSize, Point(1, 1));
		morphologyEx(edges, edges, MORPH_CLOSE, element);

		Mat largestCon = findLargestContour(edges.clone());
		line(largestCon, Point(R.x, R.y + R.height), Point(R.x + R.width, R.y + R.height), Scalar(255), 2, 8, 0);
		Mat mask = findLargestContour(largestCon.clone());

		Mat A;
		frame.copyTo(A, mask);
		imshow("original", frame);
		imshow("dst", A);
		imshow("mask", mask);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
