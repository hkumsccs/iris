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

using namespace cv;
using namespace std;

int main()
{
  int kernel_size = 31;
  double sig = 1, th = 0.3, lm = 1.0, gm = 0.02, ps = 0.2;
  while(1)
  {
    Mat in = imread("../data/f2.jpg",0);          // load grayscale
    Mat dest;
    Mat src_f;
    in.convertTo(src_f,CV_32F);

    int kernel_size = 31;
    cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size,kernel_size), sig, th, lm, gm, ps);
    cv::Mat k = GaborFilter::GaborKernel(lm, th, ps, sig, gm, kernel_size);
    cv::filter2D(src_f, dest, CV_32F, kernel);

    //Mat viz;
    //dest.convertTo(viz,CV_8U,1.0/255.0);     // move to proper[0..255] range to show it
    imshow("k", dest);

    Util::Convolve(src_f, k);
    //src_f.convertTo(viz, CV_8U, 1.0/255.0);
    imshow("d", src_f);
    
    waitKey(3);

    //cerr << dest(Rect(30,30,10,10)) << endl; // peek into the data
  }
  return 0;
}
