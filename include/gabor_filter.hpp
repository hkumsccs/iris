#include "math.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

namespace GaborFilter
{
  // https://en.wikipedia.org/wiki/Gabor_filter
  // 2D Gabor filter is a Gaussian kernel function modulated by a sinusoidal plane wave. 
  cv::Mat GaborKernel(double lambda, double theta, double psi, double sigma, double gamma, int kernel_size);
}
