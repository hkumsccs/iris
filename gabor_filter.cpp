#include "iostream"
#include "include/util.hpp"
#include <math.h>
#include "include/gabor_filter.hpp"

namespace GaborFilter
{
  // Reimplement the CV kernel, more light-weight 
  // https://github.com/opencv/opencv/blob/2f4e38c8313ff313de7c41141d56d945d91f47cf/modules/imgproc/src/gabor.cpp
  cv::Mat GaborKernel(double lambda, double theta, double psi, double sigma, double gamma, int kernel_size)
  {
    cv::Mat _kernel(kernel_size, kernel_size, CV_32F);
    const int half_size = kernel_size / 2;
    const double cth = cos(theta);
    const double sth = sin(theta);
    const double exp_const = -.5 * sigma * sigma;
    const double cos_const = 2 * CV_PI / lambda;
    const double gamma_2 = gamma * gamma;
    // Parameters for rotation 
    double _theta_x, _theta_y, _ysth, _ycth;

    for (int y = -half_size; y <= half_size; ++y)
    {
      _ysth = y * sth;
      _ycth = -y * cth;
      for (int x = -half_size; x <= half_size; ++x)
      {
        _theta_x = x * cth + _ysth;
        _theta_y = x * sth + _ycth;
        _kernel.at<float>(half_size - y, half_size - x) = (float) exp(exp_const * (_theta_x * _theta_x + gamma_2 * _theta_y * _theta_y )) * cos(cos_const * _theta_x + psi);
      }
    }
    return _kernel;
  }
}
