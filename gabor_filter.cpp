#include "iostream"
#include "include/util.hpp"
#include <math.h>
#include "include/gabor_filter.hpp"

namespace GaborFilter
{
  cv::Mat GaborKernel(double lambda, double theta, double psi, double sigma, double gamma, int kernel_size)
  {
    cv::Mat _kernel(kernel_size, kernel_size, CV_32F);
    // Symmetry, just like to build Gussian filter
    int half_size = (kernel_size - 1) / 2
    
    double _theta = Util::Degree2Radian(theta);
    double _psi = Util::Degree2Radian(psi);
    double _sigma = sigma / kernel_size;

    // Parameters for rotation 
    double _theta_x, _theta_y;

    for (int y = -half_size; y <= half_size; ++y)
      for (int x = -half_size; x <= half_size; ++x)
      {
        _theta_x = (x * cos(_theta) + y * sin(_theta)) / _theta;
        _theta_y = (-x * sin(_theta) + y * cos(_theta)) / _theta;
        _kernel.at<float>() = (float) exp(- (_theta_x * theta_x + _theta_y * _theta_y) / (2 * sigma * sigma)) * cos(2 * CV_PI * _theta_x / lambda + _psi);
      }

    return _kernel;
  }
}
