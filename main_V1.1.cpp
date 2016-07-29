#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;

	// Load cascate classifier from the library
	CascadeClassifier detector;
	string cascadeName = "haarcascade_frontalface_alt.xml";
	bool loaded = detector.load(cascadeName);

	// Vector of returned faces
	vector<Rect> found;

	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY); //transform to a grayscale image

		// Detect faces
		detector.detectMultiScale(edges, found, 1.1, 2, 0 | 2, Size(80,80), Size(200,200));

		// Draw the results into mat retrieved from webcam
		if (found.size() > 0) {
			for (int i = 0; i <= found.size() - 1; i++) {
				rectangle(edges, found[i].br(), found[i].tl(), Scalar(0, 0, 0), 1, 8, 0);
				rectangle(frame, found[i].br(), found[i].tl(), Scalar(0, 0, 0), 1, 8, 0);
			}
		}

		// Using Canny Edge Detection
		Canny(edges, edges, 30, 255);

		imshow("original", frame);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
