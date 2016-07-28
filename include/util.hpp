#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <list>

namespace Util
{
  cv::Mat BoxFilter (float size = 3.0);
  cv::Mat GradientImage(cv::Mat& f, int width, int height); 
  void Convolve (cv::Mat& f, cv::Mat& w, int width, int height, int a, int b);
  void Smoothing (cv::Mat& f, double sigma, int width, int height);
};
