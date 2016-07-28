#include "iostream"
#include "include/corners.hpp"
#include "include/util.hpp"
#include <math.h>

using namespace std;

namespace CornerDetector
{
   std::list<std::pair<double, double>> FindCorners(Mat& f, int width, int height, double sigma, double threshold)
   {
     std::list<std::pair<double, double>> corner_list;

     cv::Mat R = ComputeR(f, sigma, threshold, width, height);

     // Sub-pixel approximation
     for (int y = 1; y < height - 1; ++y)
     {
       for (int x = 1; x < width - 1; ++x)
       {
         double val = R.at<uint8_t>(x, y);
         if(val > threshold && val > R.at<uint8_t>(x-1, y-1) && val > R.at<uint8_t>(x, y-1)
            && val > R.at<uint8_t>(x+1, y-1) && val > R.at<uint8_t>(x-1, y)
            && val > R.at<uint8_t>(x+1, y) && val > R.at<uint8_t>(x-1, y+1)
            && val > R.at<uint8_t>(x, y+1) && val > R.at<uint8_t>(x+1, y+1))
         {
           double corner_x = x + .5 + (R.at<uint8_t>(x-1, y) - R.at<uint8_t>(x+1, y))
                              / (R.at<uint8_t>(x-1, y) + R.at<uint8_t>(x+1, y) - 2 * val) / 2;
           double corner_y = y + .5 + (R.at<uint8_t>(x, y-1) - R.at<uint8_t>(x, y+1))
                              / (R.at<uint8_t>(x, y-1) + R.at<uint8_t>(x, y+1) - 2 * val) / 2;

           corner_list.push_back(std::make_pair(corner_x, corner_y));
         }
       }
     }

     return corner_list;
   }

   cv::Mat ComputeR(Mat& f, double sigma, double threshold, int width, int height, double k = 0.04)
   {
     // Initialization
     cv::Mat fx2 = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);
     cv::Mat fy2 = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);
     cv::Mat fxy = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);
     cv::Mat R = cv::Mat::zeros(f.rows, f.cols, CV_8UC1);

     // Compute the auto-correlation matrix
     for (int x = 1; x < height - 1; ++x)
     {
       for (int y = 1; y < width -1; ++y)
       {
         int fx = f.at<uint8_t>(x+1, y) - f.at<uint8_t>(x-1, y);
         int fy = f.at<uint8_t>(x, y+1) - f.at<uint8_t>(x, y-1);
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
         double det_A = fx2.at<uint8_t>(x, y) * fy2.at<uint8_t>(x, y) - fxy.at<uint8_t>(x, y) * fxy.at<uint8_t>(x,y);
         double trace_A = fx2.at<uint8_t>(x, y) + fy2.at<uint8_t>(x, y);
         R.at<uint8_t>(x, y) = det_A - k * trace_A * trace_A;
       }
     }
     
     return R;
   }

}