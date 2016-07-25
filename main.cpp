#include "iostream"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

int main()
{
  VideoCapture cap(0);

  if(!cap.isOpened())
  {
    cout << "Webcam is not opened" << endl;
  }

  while(1)
  {
    Mat image;
    Mat HSVimage;
    Mat processedImage;

    // Assign mat image to the raw webcam footage
    cap.read(image);
    imshow("Test", image);

  }

}
