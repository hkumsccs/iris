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

int main()
{

  while(1)
  {
    cv::Mat in = cv::imread("../data/f2.jpg", 0);          // load grayscale
    cv::Mat dest, dest1;
    cv::Mat src_f;
    in.convertTo(src_f, CV_32F);

    int kernel_size = 31;
    double sig = 1.0, th = 0.2, lm = 1.0, gm = 0.02, ps = 0.0;
    cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size,kernel_size), sig, th, lm, gm, ps);
    
    //cv::Mat GaborKernel(double lambda, double theta, double psi, double sigma, double gamma, int kernel_size);
    cv::Mat k = GaborFilter::GaborKernel(lm, th, ps, sig, gm, kernel_size);

    //cerr << dest(Rect(30,30,10,10)) << endl; // peek into the data
    cv::filter2D(src_f, dest1, CV_32F, kernel);
    Util::Convolve(src_f, k, dest);

    cv::Mat viz;
    dest.convertTo(viz, CV_8U, 1.0/255.0);     // move to proper[0..255] range to show it

    cv::Mat viz1;
    dest1.convertTo(viz1, CV_8U, 1.0/255.0);     // move to proper[0..255] range to show it
    imshow("k",kernel);
    imshow("my",k);
    imshow("my dest", viz);
    imshow("k dest", viz1);
    cv::waitKey();
  }
  return 0;
}
