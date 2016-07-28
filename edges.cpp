#include "include/edges.hpp"
#include "iostream"
#include <math.h>

using namespace std;

namespace EdgeDetector
{
  cv::Mat GradientImage(cv::Mat& f, int width, int height)
  {
    // scale it to 0-255
    cv::Mat gradient = f.clone();
    double c = 255 / sqrt(130050);

    for(int x=1; x<height-1; ++x)
    {
      for(int y=1; y<width-1; ++y)
      {
        int fx = f.at<uint8_t>(x+1, y) - f.at<uint8_t>(x-1, y);
        int fy = f.at<uint8_t>(x, y+1) - f.at<uint8_t>(x, y-1);
        gradient.at<uint8_t>(x, y) = c * sqrt(fx*fx + fy*fy);
      }
    }
    return gradient;
  }
}
