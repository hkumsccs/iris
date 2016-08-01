#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <list>

namespace Util
{
  std::pair<int, int> FindIris(cv::Mat& f);
  cv::Mat BoxFilter (float size);
  cv::Mat GradientImage(cv::Mat& f, int width, int height); 
  cv::Mat InvertedImage(cv::Mat& f, int width, int height, int threshold); 
  void Convolve (cv::Mat& f, double* w, int width, int height, int a, int b);
  void Convolve (cv::Mat& f, cv::Mat& w);
  cv::Mat FilterMat2D(cv::Mat& input, cv::Mat& filter);
  void Smoothing (cv::Mat& f, double sigma, int width, int height);
  double Degree2Radian(double deg);
  int GetHistInfo(int hist[], int size, bool is_return_index);
  void Array2String(int a[], int size);
};
