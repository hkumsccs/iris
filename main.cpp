#include <list>
#include "iostream"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "util.cpp"
#include "corners.cpp"

using namespace cv;
using namespace std;

#define KERNEL_SIZE 
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

  while(1)
  {
    // Assign mat image to the raw webcam footage
    cap.read(raw_image);
    //raw_image = imread("./f1.jpeg", 1);
    cvtColor(raw_image, gray_image, CV_BGR2GRAY);
    //imshow("raw", raw_image);

    //std::list<std::pair<double, double> > c_list = CornerDetector::FindCorners(gray_image, gray_image.cols, gray_image.rows, 0.5, 4);
    //cout << c_list.size() << endl;

    medianBlur(gray_image, blur_image, 5);

    //blur_image = Util::GradientImage(gray_image, gray_image.cols, gray_image.rows) > 30;
   
    imshow("Test", CornerDetector::CornerImage(blur_image, 0.7, 4, gray_image.cols, gray_image.rows));
    
    //imshow("Test", blur_image);
    waitKey(50);
  }

}
