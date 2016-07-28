#include "include/util.hpp"
#include "iostream"
#include <algorithm>
#include <math.h>

using namespace std;

#define GUSSIAN_DIE_OFF 0.001

namespace Util
{
  cv::Mat BoxFilter(float size)
  {
    return cv::Mat::ones(size, size, CV_8UC1) / (size * size);
  }

  cv::Mat GradientImage(cv::Mat& f, int width, int height)
  {
    // scale it to 0-255
    cv::Mat gradient = f.clone();
    double c = 255 / sqrt(130050);

    for(int x=1; x<height-1; ++x)
    {
      for(int y=1; y<width-1; ++y)
      {
        int fx = f.at<uint8_t>(x+1, y) - f.at<uint8_t>(x-1, y);
        int fy = f.at<uint8_t>(x, y+1) - f.at<uint8_t>(x, y-1);
        gradient.at<uint8_t>(x, y) = c * sqrt(fx*fx + fy*fy);
      }
    }
    return gradient;
  }
   
  void Convolve (cv::Mat& f, double* w, int width, int height, int a, int b)
  {
    cv::Mat f2 = f.clone();
    int filter_size = min(2*a+1, 2*b+1);
    for (int x = a; x < height - a; ++x)
    {
      for (int y = b; y < width - b; ++y)
      {
        double sum = 0.0;
        for (int s = -a; s <= a; ++s)
        {
          for (int t = -b; t <= +b; ++t)
          {
            sum += w[(s+a) * filter_size + t + b] * (double)f2.at<uint8_t>(x+s, y+t);
          }
          f.at<uint8_t>(x, y) = sum;
        }
      }
    }
  }

  void Convolve (cv::Mat& f, cv::Mat& w, int width, int height, int a, int b)
  {
    cv::Mat f2 = f.clone();
    int filter_size = min(2*a+1, 2*b+1);
    for (int x = a; x < height - a; ++x)
    {
      for (int y = b; y < width - b; ++y)
      {
        double sum = 0.0;
        for (int s = -a; s <= a; ++s)
        {
          for (int t = -b; t <= +b; ++t)
          {
            sum += (double)w.at<uint8_t>(s+a, t+b) * (double)f2.at<uint8_t>(x+s, y+t);
          }
          f.at<uint8_t>(x, y) = sum;
        }
      }
    }
  }

  void Smoothing (cv::Mat& f, double sigma, int width, int height)
  {
    double sum_half_mask = 0;

    // Generate a 1D Gussian filter such that the discarded samples are less than 1/1000 of the peak value
    list<double> half_mask;
    for (int i = 0; true; ++i)
    {
      double d = exp(-i * i / (2.0 * sigma * sigma));
      if(d < GUSSIAN_DIE_OFF)
      {
        break;
      }
      else
      {
        sum_half_mask += d;
        half_mask.push_front(d);
      }
    }

    list<double> full_mask = list<double>(half_mask);
    half_mask.reverse();
    half_mask.pop_front();
    full_mask.insert(full_mask.end(), half_mask.begin(), half_mask.end());

    // Transform the data structure from list<double> to Mat
    //cv::Mat mask(1, full_mask.size(), cv::DataType<double>::type);
    double mask[full_mask.size()];
    int iter = 0;
    while (!full_mask.empty())
    {
      // Trick: After multiply by 2, need to substract one for the peak value of Gussian filter
      //mask.at<uint8_t>(iter++, 1) = (double)(full_mask.front() / (sum_half_mask * 2 - 1));
      mask[iter++] = (double)(full_mask.front() / (sum_half_mask * 2 - 1));
      full_mask.pop_front();
    }

    // Decompose 2D Gussian masking into executing 1D Gussian masking for twice
    Convolve(f, mask, width, height, 2, 0); 
    Convolve(f, mask, width, height, 0, 2); 
  }
  
}
