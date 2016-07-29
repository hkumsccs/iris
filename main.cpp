#include <list>
#include "iostream"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "util.cpp"
#include "corners.cpp"
#include "opencv2/video/video.hpp"

using namespace cv;
using namespace std;

#define KERNEL_SIZE 3
#define SIGMA_X 4 
/*
 * All the references
 *
 * http://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#performance-difference
 *
*/

int main()
{
  VideoCapture cap(0);

  if(!cap.isOpened())
  {
    cout << "Webcam is not opened" << endl;
  }

  Mat raw_image;
  Mat gray_image;
  Mat blur_image;

  //Size ksz = Size(KERNEL_SIZE, KERNEL_SIZE);
  CascadeClassifier face_detector;
  //string cascade_name = "haarcascade_frontalface_alt.xml";
  face_detector.load("./metadata/haarcascade_frontalface_alt.xml");
  // Vector of returned faces
	vector<Rect> found;

  Mat edges;

  while(1)
  {
    // Assign mat image to the raw webcam footage
    cap.read(raw_image);
    //raw_image = imread("./f1.jpeg", 1);
    cvtColor(raw_image, gray_image, CV_BGR2GRAY);
    //imshow("raw", raw_image);

		cvtColor(raw_image, edges, CV_BGR2GRAY); //transform to a grayscale image

		// Detect faces
		face_detector.detectMultiScale(edges, found, 1.1, 2, 0 | 2, Size(80,80), Size(200,200));

		// Draw the results into mat retrieved from webcam
		if (found.size() > 0) {
			for (int i = 0; i <= found.size() - 1; i++) {
				rectangle(edges, found[i].br(), found[i].tl(), Scalar(0, 0, 0), 1, 8, 0);
				rectangle(raw_image, found[i].br(), found[i].tl(), Scalar(0, 0, 0), 1, 8, 0);
			}
		}

		imshow("original", raw_image);
		if (waitKey(30) >= 0) break;

    //std::list<std::pair<double, double> > c_list = CornerDetector::FindCorners(gray_image, gray_image.cols, gray_image.rows, 0.5, 4);
    //cout << c_list.size() << endl;
    //medianBlur(gray_image, blur_image, 3);
    //blur_image = Util::GradientImage(gray_image, gray_image.cols, gray_image.rows) > 30;
    //imshow("Test", CornerDetector::CornerImage(gray_image, 0.7, 3, gray_image.cols, gray_image.rows));
    //imshow("Test", blur_image);
  }

}
