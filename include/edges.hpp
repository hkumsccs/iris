#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/gpu/gpu.hpp"

namespace EdgeDetector
{
  void test();
  cv::Mat GradientImage(cv::Mat& f, int width, int height); 
};
