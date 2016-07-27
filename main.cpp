#include "iostream"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/gpu/gpu.hpp"

using namespace cv;
using namespace std;

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

int main()
{
  VideoCapture cap(0);

  if(!cap.isOpened())
  {
    cout << "Webcam is not opened" << endl;
  }

  Mat kernel = initialize_box_filter();
  Mat raw_image;
  Mat gray_image;

  while(1)
  {
    // Assign mat image to the raw webcam footage
    cap.read(raw_image);
    cvtColor(raw_image, gray_image, CV_BGR2GRAY);
    Mat smoothed_image = em_convolve(gray_image, kernel, gray_image.cols, gray_image.rows);
    imshow("Test", smoothed_image);
    waitKey(1);
  }

}
