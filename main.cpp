#include "iostream"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/gpu/gpu.hpp"
#include "edges.cpp"

using namespace cv;
using namespace std;

#define KERNEL_SIZE 13 
#define SIGMA_X 3
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

  Mat kernel = initialize_box_filter();
  Mat raw_image;
  Mat gray_image;
  Mat blur_image;

  Size ksz = Size(KERNEL_SIZE, KERNEL_SIZE);

  while(1)
  {
    // Assign mat image to the raw webcam footage
    cap.read(raw_image);
    cvtColor(raw_image, gray_image, CV_BGR2GRAY);
    //GaussianBlur(gray_image, blur_image, ksz, SIGMA_X);
    //imshow("Test", EdgeDetector::GradientImage(blur_image, blur_image.cols, blur_image.rows));
    waitKey(1);
  }

}
