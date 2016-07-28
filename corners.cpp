#include "iostream"
#include "include/corners.hpp"
#include "include/util.hpp"
#include <math.h>

using namespace std;

namespace CornerDetector
{
  cv::Mat CornerImage(cv::Mat& f, double sigma, double threshold, int width, int height)
  {
    double R[720 * 1280] = {0};
    ComputeR(f, sigma, threshold, width, height, R);
    cv::Mat f2 = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);

    for (int x = 0; x < height - 1; ++x)
    {
      for (int y = 0; y < width - 1; ++y)
      {
        f2.at<uint8_t>(x, y) = R[x*width + y] > threshold ? 255 : 0;
      }
    }
    
    return f2;
  }
  
  std::list< std::pair<double, double> > FindCorners(cv::Mat& f, int width, int height, double sigma, double threshold)
  {
    std::list<std::pair<double, double> > corner_list;

    double R[ width*height ];
    ComputeR(f, sigma, threshold, width, height, R);

    // Sub-pixel approximation
    for (int y = 1; y < height - 1; ++y)
    {
      for (int x = 1; x < width - 1; ++x)
      {
        int iwj = y * width + x;
        double val = R[iwj];
        if(val > threshold && val > R[iwj - width - 1] && R[iwj - width]
          && val > R[iwj - width + 1] && val > R[iwj - 1]
          && val > R[iwj + 1] && val > R[iwj + width - 1]
          && val > R[iwj + width] && val > R[iwj + width + 1])
        {
          double corner_x = x + .5 + (R[iwj - 1] - R[iwj + 1])
                            / (R[iwj - 1] + R[iwj + 1] - 2 * val) / 2;
          double corner_y = y + .5 + (R[iwj - width] - R[iwj + width])
                            / (R[iwj - width] + R[iwj + width] - 2 * val) / 2;

          corner_list.push_back(std::make_pair(corner_x, corner_y));
        }
      }
    }

    return corner_list;
  }

  void ComputeR(cv::Mat& f, double sigma, double threshold, int width, int height, double* R, double k)
  {
    // Initialization
    cv::Mat fx2 = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);
    cv::Mat fy2 = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);
    cv::Mat fxy = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);
    // cv::Mat R = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);
    // Compute the auto-correlation matrix
    for (int x = 1; x < height - 1; ++x)
    {
      for (int y = 1; y < width -1; ++y)
      {
        int fx = (int)f.at<uint8_t>(x+1, y) - (int)f.at<uint8_t>(x-1, y);
        int fy = (int)f.at<uint8_t>(x, y+1) - (int)f.at<uint8_t>(x, y-1);
        fx2.at<uint8_t>(x, y) = fx * fx;
        fy2.at<uint8_t>(x, y) = fy * fy;
        fxy.at<uint8_t>(x, y) = fx * fy;
      }
    }

    Util::Smoothing(fx2, sigma, width, height);
    Util::Smoothing(fy2, sigma, width, height);
    Util::Smoothing(fxy, sigma, width, height);

    for (int x = 1; x < height - 1; ++x)
    {
      for (int y = 1; y < width - 1; ++y)
      {
        double det_A = (double)fx2.at<uint8_t>(x, y) * (double)fy2.at<uint8_t>(x, y) - (double)fxy.at<uint8_t>(x, y) * (double)fxy.at<uint8_t>(x,y);
        double trace_A = (double)fx2.at<uint8_t>(x, y) + (double)fy2.at<uint8_t>(x, y);
        //R.at<uint8_t>(x, y) = det_A - k * trace_A * trace_A;
        R[x*width+y] = det_A - k * trace_A * trace_A;
      }
    }
    cout << R << endl;
  }

}
