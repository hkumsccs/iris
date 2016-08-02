#include "stdio.h"
#include "math.h"
#include "opencv/cv.h"
#include <opencv/highgui.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "../gabor_filter.cpp"
#include "../util.cpp"

using namespace std;
using namespace cv;

cv::Mat in;
cv::Mat src_f;
cv::Mat dest;
cv::Mat dst;
cv::Mat kernel;
cv::Mat gray_img;

int kernel_size = 13;
int slider_sig;
int slider_th;
int slider_lm;
int slider_gm;
int slider_ps;

double sig = 1.0, th = 0.79, lm = 1.0, gm = 5.0, ps = 0.4;

void processing()
{
  static int index = 0;
  //kernel = cv::getGaborKernel(cv::Size(kz,kz), sig, th, lm, gm, ps);
  //cv::Mat GaborKernel(double lambda, double theta, double psi, double sigma, double gamma, int kernel_size)
  kernel = GaborFilter::GaborKernel(lm, th, ps, sig, gm, kernel_size);

  //cv::filter2D(gray_img, dest, CV_32F, kernel);
  dest = Util::FilterMat2D(gray_img, kernel);

  //dest.convertTo(dst, CV_8U, 1.0/255.0);     // move to proper[0..255] range to show it
  imshow( "gf", dest);
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

  namedWindow("gf", 1);

  slider_sig = 1;
  slider_th = 0;
  slider_gm = 2;
  slider_lm = 3;
  slider_ps = 0;

  createTrackbar("Sigma", "gf", &slider_sig, 20, on_trackbar_sig);
  createTrackbar("Theta", "gf", &slider_th, 180, on_trackbar_th);
  createTrackbar("Lambda", "gf", &slider_lm, 12, on_trackbar_lm);
  createTrackbar("Gamma", "gf", &slider_gm, 10, on_trackbar_gm);
  createTrackbar("Psi", "gf", &slider_ps, 180, on_trackbar_ps);

  on_trackbar_sig(slider_sig, 0 );

  while(true)
  {
    capture >> in;
    //in = imread("./image/red.jpg");
    waitKey(1);
    cvtColor(in, gray_img, CV_BGR2GRAY);

    // Wait until key press 
    filtering(kernel_size, sig, th, lm, gm, ps);
    char c = cvWaitKey(3);
    if( c == 27 ) break;
    //waitKey(0);
  }
  return 0;
}
