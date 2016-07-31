#include "stdio.h"
#include "math.h"
#include "opencv/cv.h"
#include <opencv/highgui.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "gabor_filter.cpp"

using namespace cv;
using namespace std;

int main()
{
  int kernel_size = 31;
  double sig = 1, th = 0, lm = 1.0, gm = 0.02, ps = 0;
  while(1)
  {
    cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, th, lm, gm, ps);
    //cv::Mat GaborKernel(double lambda, double theta, double psi, double sigma, double gamma, int kernel_size)
    cv::Mat k = GaborFilter::GaborKernel(lm, th, ps, sig, gm, kernel_size);

    //cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps)
    //cv::Mat k1 = GaborFilter::mkKernel(kernel_size, sig, th, lm, ps);
    imshow("Gabor", kernel);
    imshow("k", k);
    //imshow("k1", k1);
    waitKey(1);
  }
  return 0;
}
