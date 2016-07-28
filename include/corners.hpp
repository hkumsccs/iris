#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/gpu/gpu.hpp"

namespace CornerDetector
{
   std::list<Point2D> FindCorners(Mat& f, int width, int height, double sigma, double threshold); 
   cv::Mat ComputeR(Mat& f, double sigma, double threshold, int width, int height);
};
