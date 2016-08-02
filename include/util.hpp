#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <list>

namespace Util
{
  std::pair<int, int> FindIris(cv::Mat& f);
  cv::Mat BoxFilter (float size);
  cv::Mat MedianFilter(const cv::Mat& f, int size);
  cv::Mat GradientImage(cv::Mat& f, int width, int height); 
  cv::Mat InvertedImage(cv::Mat& f, int width, int height, int threshold); 
  cv::Mat FilterMat2D(const cv::Mat& f, const cv::Mat &w);
  void Convolve (cv::Mat& f, double* w, int width, int height, int a, int b);
  cv::Mat Convolve (const cv::Mat& f, const cv::Mat& w);
  void Smoothing (cv::Mat& f, double sigma, int width, int height, int size);
  double Degree2Radian(double deg);
  int GetHistInfo(int hist[], int size, bool is_return_index);
  void Array2String(int a[], int size);
  cv::Mat SkinColorSegmentation(cv::Mat& f);
  float MeanAmplitude( cv::Mat& f, int width, int height);
  float LocalEnergy ( cv::Mat& f, int width, int height);
};
