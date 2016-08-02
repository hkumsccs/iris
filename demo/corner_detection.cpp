#include "stdio.h"
#include "math.h"
#include "opencv/cv.h"
#include <opencv/highgui.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "../util.cpp"
#include "../corners.cpp"

using namespace std;
using namespace cv;

cv::Mat in;
cv::Mat src_f;
cv::Mat dest;
cv::Mat dst;
cv::Mat viz;
cv::Mat kernel;
cv::Mat gray_img;
cv::Mat tmp_gray_img;

int kernel_size = 13;
int slider_r;
int slider_sig;
int slider_size;
int slider_msize;

double r = 0.04;
double sigma = 0.7;
double g_size = 5;
double m_size = 1;

void corner(int kz, double r, int s, int ms)
{
  static int index = 0;

  /*
  printf(" Frame %d ============ \n", index++);
  printf(" Kernel size : %d\n", kz);
  printf(" R           : %f\n", r);
  printf(" Sigma       : %f\n", sigma);
  printf(" Gussian     : %f\n", g_size);
  printf(" Median      : %f\n", m_size);
  printf(" ===================== \n\n\n");
  */

  gray_img = Util::MedianFilter(gray_img, ms);
  
  dest = CornerDetector::CornerImage(gray_img, 0.7, r, gray_img.cols, gray_img.rows, s);
  
  Size sz1 = gray_img.size();
  Size sz2 = dest.size();
  Mat im3(sz1.height, sz1.width+sz2.width, CV_8UC1);
  Mat left(im3, Rect(0, 0, sz1.width, sz1.height));
  gray_img.copyTo(left);
  Mat right(im3, Rect(sz1.width, 0, sz2.width, sz2.height));
  dest.copyTo(right);
  imshow("corner", im3);
}

void on_trackbar_r( int, void* )
{
  r = slider_r / 100.0;
  cout << "Change R to " << r << endl;
}

void on_trackbar_sig( int, void* )
{
  sigma = slider_sig / 100.0;
  cout << "Change Sigma to " << sigma << endl;
}

void on_trackbar_size( int, void* )
{
  g_size = slider_size * 2 + 3;
  cout << "Change gussian size to " << g_size << endl;
}

void on_trackbar_msize( int, void* )
{
  m_size = slider_msize * 2 + 1;
  cout << "Change median size to " << m_size << endl;
}

int main()
{
	VideoCapture capture(0); //-1, 0, 1 device id
	if(!capture.isOpened())
	{
		printf("error to initialize camera");
		return 1;
	}
  capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);  
  capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);  
  // ----------------------------------------------

  namedWindow("corner", 1);

  slider_r = 40;
  slider_sig = 40;
  slider_size = 1;
  slider_msize = 0;

  createTrackbar("R", "corner", &slider_r, 2500, on_trackbar_r);
  createTrackbar("Sigma", "corner", &slider_sig, 100, on_trackbar_sig);
  createTrackbar("Gussian size", "corner", &slider_size, 5, on_trackbar_size);
  createTrackbar("Median filter size", "corner", &slider_msize, 3, on_trackbar_msize);

  on_trackbar_r(slider_r, 0);

  while(true)
  {
    //capture.read(in);
    //in = imread("./image/checkerboard.jpg");
    in = imread("./image/cameraman.jpg");
    waitKey(1);
    cvtColor(in, gray_img, CV_BGR2GRAY);
    waitKey(1);

    // Wait until key press 
    corner(kernel_size, r, g_size, m_size);

    char c = cvWaitKey(3);
    if( c == 27 ) break;
  }
  return 0;
}
