#include "include/util.hpp"
#include "iostream"
#include <algorithm>
#include <string>
#include <math.h>

using namespace std;
using namespace Util;

#define GUSSIAN_DIE_OFF 0.001

namespace Util
{
  cv::Mat BoxFilter(float size)
  {
    return cv::Mat::ones(size, size, CV_8UC1) / (size * size);
  }

  cv::Mat InvertedImage(cv::Mat& f, int width, int height, int threshold = -1)
  {
    cv::Mat inverted = f.clone();
    int inverted_val = -1;
    for(int x=1; x<height-1; ++x)
      for(int y=1; y<width-1; ++y)
      {
        inverted_val = 255 - (int)f.at<uint8_t>(x, y);
        if(threshold != -1)
        {
          inverted.at<uint8_t>(x, y) = inverted_val > threshold ? 255 : 0;
        }
        else
        {
          inverted.at<uint8_t>(x, y) = inverted_val;
        }
      }
    return inverted;
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
        }
        f.at<uint8_t>(x, y) = sum;
      }
    }
  }

  cv::Mat FilterMat2D(const cv::Mat& f, const cv::Mat &w)
  {
    cv::Mat f1 = f.clone();
    cv::Mat w1 = w.clone();
    uchar * data = f.data;
    
    cv::flip(w, w1, -1);

    //w.convertTo(w, CV_32F);
    float * kernel_data = (float *)w.data;

    int a = w.cols / 2;

    for (int x = a; x < f.rows - a; ++x)
      for (int y = a; y < f.cols - a; ++y)
      {
        float sum = 0.0;
        for (int s = -a; s <= a; ++s)
          for(int t = -a; t <= +a; ++t)
          {
            if(s+a == 3)
            {
              //cout << "s+a " << s+a << " y+t " << (y+t)%w.cols << "   " << (float) kernel_data[(s+a)*w.cols+(t+b)] << endl;
            }
            //sum += (float) kernel_data[(s+a)*w.cols+(t+b)] * (float)data[(x+s)*f.cols+(y+t)];
            sum += w1.at<float>(s+a, t+a) * data[(x + s)*f.cols+(y + t)];
          }

        f1.data[x * f.cols + y] = max(0, min(255, (int)sum));
      }
    return f1;
  }

  cv::Mat Convolve (const cv::Mat& f, const cv::Mat& w)
  {
    cv::Mat output = f.clone();

    int height = f.rows;
    int width = f.cols;
    int a = (w.rows - 1) / 2;
    int b = (w.cols- 1) / 2;

    cv::Mat f2 = f.clone();

    for (int x = 0; x < height; ++x)
    {
      for (int y = 0; y < width; ++y)
      {
        float sum = 0.0;
        if(x >= a && x < height - a && y >= b && y < width - b )
        {
          for (int s = -a; s <= a; ++s)
          { 
            for (int t = -b; t <= b; ++t)
            {
              sum += (float)w.at<double>(s+a, t+b) * (float)f2.at<double>(x+s, y+t);
            }
          }
        }
        else
        {
          // zero padding
          sum = f2.at<float>(x, y) * 255.0;
        }
        output.at<float>(x, y) = sum;
      }
    }
    return output;
  }

  void Smoothing (cv::Mat& f, double sigma, int width, int height, int size = 5)
  {
    double sum_half_mask = 0;

    // Generate a 1D Gussian filter such that the discarded samples are less than 1/1000 of the peak value
    list<double> half_mask;

    double x = (size + 1) / 2;
    double gussian_die_off = exp(- x * x / (2 * sigma * sigma));
    
    for (int i = 0; true; ++i)
    {
      double d = exp(-i * i / (2.0 * sigma * sigma));
      if(d < gussian_die_off)
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

    int ss = full_mask.size() / 2;

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
    
    Convolve(f, mask, width, height, ss, 0); 
    Convolve(f, mask, width, height, 0, ss); 
  }

  int GetHistInfo(int hist[], int size, bool is_return_index = true)
  {
    int max = 0;
    int index = 0;

    for(int i = 0; i < size; ++i)
    {
      if(hist[i] > max)
      {
        max = hist[i];
        //cout << std::to_string(max) << " !!! " << std::to_string(i) << endl;
        index = i;
      }
    }

    return is_return_index ? index : max;
  }

  std::pair<int, int> FindIris(cv::Mat& f)
  {
    double max = -1.0;
    int index_col = 0;
    int index_row = 0;
    cv::Mat diff_back, diff_front; 

    int hist_col[320] = {0};
    int hist_row[240] = {0};

    for(int j=1; j < f.cols - 1; ++j)
    {
      cv::Mat successive_back_data_col = f.col(j-1);
      cv::Mat data_col = f.col(j);
      cv::absdiff(successive_back_data_col, data_col, diff_back);
      for(int i=0; i < diff_back.rows; ++i)
      {
        //hist_col[(int)diff_back.at<uint8_t>(j, i)]++; 
        hist_col[j] += (int)diff_back.at<uint8_t>(j, i);
      }
    }

    max = -1.0;

    for(int j=1; j < f.rows - 1; ++j)
    {
      cv::Mat successive_back_data_row = f.row(j-1);
      cv::Mat data_row = f.row(j);
      cv::absdiff(successive_back_data_row, data_row, diff_back);
      for(int i=0; i < diff_back.cols; ++i)
      {
        //hist_row[(int)diff_back.at<uint8_t>(i, j)]++; 
        hist_row[j] += (int)diff_back.at<uint8_t>(i, j);
      }
    }

    index_col = GetHistInfo(hist_col, f.cols);
    index_row = GetHistInfo(hist_row, f.rows);

    //Array2String(hist_col, 256);

    return std::make_pair(index_col, index_row);
  }

  double Degree2Radian(double deg)
  {
    return deg * CV_PI / 180;
  }

  void Array2String(int a[], int size) {
    std::string _o = "";
    for (int i = 0; i < size; i++)
      _o += std::to_string(a[i]) + " ";
    cout << _o << endl;
  }

  cv::Mat MedianFilter(const cv::Mat& f, int size)
  {
    cv::Mat f1 = f.clone();
    uchar * data = f.data;
    vector<int> values;

    int a = size / 2;

    for (int x = a; x < f.rows - a; ++x)
      for (int y = a; y < f.cols - a; ++y)
      {
        int i = 0;
        for (int s = -a; s <= a; ++s)
          for(int t = -a; t <= +a; ++t)
            values.push_back(data[(x + s) * f.cols + (y + t)]);

        std::sort(values.begin(), values.end());
        f1.data[x * f.cols + y] = values[size * size / 2];
        values.clear();
      }
    return f1;
  }

  float MeanAmplitude( cv::Mat& f, int width, int height){
    float sum = 0.0;
    for(int i = 0; i < width; ++i){
      for(int j = 0; j < height; ++j){
        sum += f.at<uchar>(j, i) / 255.0;
      }
    }
    return sum;
  }

  float LocalEnergy ( cv::Mat& f, int width, int height){
    float sum  = 0.0;
    for(int i = 0; i < width; ++i){
      for (int j = 0; j < height; ++j){
        sum += f.at<uchar>(j, i) * f.at<uchar>(j, i) / (255.0 * 255.0);
      }
    }
    return sum; 
  }

}
