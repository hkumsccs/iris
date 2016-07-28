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

/*

Mat initialize_box_filter (float size = 3.0)
{
  return Mat::ones(size, size, CV_32F) / (size * size);
}

Mat em_convolve(Mat& image, Mat& kernel, int image_cols, int image_rows)
{
  int kernel_half = kernel.rows/2.f;
  // Initialize the output image
  Mat outputMat = Mat::ones(image_rows, image_cols, image.type());

  for (int32_t y = 0; y<image_rows; ++y) 
  {
    for (int32_t x = 0; x<image_cols; ++x) 
    {
      float sum = 0;
      for (int a = -kernel_half; a <= kernel_half; ++a) 
      {
        for (int b = -kernel_half; b <= kernel_half; ++b) 
        {
          if (x+b >= 0 && x+b <= image_cols && y+a >= 0 && y <= image_rows) 
          {
            sum += kernel.at<float>(a+kernel_half, b+kernel_half)*image.at<uint8_t>(y-a+1, x-b+1);
          }
        }
      }
      outputMat.at<uint8_t>(y,x) = std::min(std::max(0.0f, sum), 255.0f);
    }
  }
  return outputMat;
}

*/
