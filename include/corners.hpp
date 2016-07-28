#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <list>

namespace CornerDetector
{
   std::list<std::pair<double, double> > FindCorners(cv::Mat& f, int width, int height, double sigma, double threshold); 
   void ComputeR(cv::Mat& f, double sigma, double threshold, int width, int height, double *R, double k = 0.04);
   cv::Mat CornerImage(cv::Mat&f, double sigma, double threshold, int width, int height);
};
