#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "opencv/cv.h"
#include <opencv/highgui.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "iostream"
#include <fstream>
#include "util.cpp"
#include "gabor_filter.cpp"
#include <string>

using namespace cv;
using namespace std;

#define blue  CV_RGB(0,0,255)
#define green CV_RGB(0,255,0)
#define red   CV_RGB(255,0,0)
#define white CV_RGB(255,255,255)
#define black CV_RGB(0,0,0)

/// Global Variables
const int alpha_slider_max = 255;
int alpha_slider = 20;
double alpha;

/// Matrices to store images
int kernel_size = 7;
double sig = 1.0, th = 0.79, lm = 1.0, gm = 5.0, ps = 0.4;

int main()
{
	CascadeClassifier face_cascade;
	if (!face_cascade.load("./metadata/haarcascade_frontalface_alt.xml")) {
		printf("Error loading cascade file for face");
		return 1;
	}

  std::string line, csvItem;
  ifstream myfile ("./training/filelist.csv");
  
  int lineNumber = 0; 
  
  std::list<std::string> file_list;
   
  if (myfile.is_open()) {
    while (getline(myfile,line)) {
      lineNumber++;
      if (!line.empty() && line[line.length()-1] == '\r') {
        line.erase(line.length()-1);
      }
      file_list.push_back("./data/jaffe/" + line); 
    }
    myfile.close();
  }

  int file_id = 0;
  
  std::string current_file;
  std::list<std::string> le_list;
  std::list<std::string> ma_list;

  std::list<std::string> feature_vec;
  std::list<std::string> valid_file_list;

  while (!file_list.empty())
  {
    current_file = file_list.front();
    
    cv::Mat in, gray_img;
    in = imread(current_file);
    cvtColor(in, gray_img, CV_BGR2GRAY);
    waitKey(10);
    vector<Rect> faces, eyes;
    face_cascade.detectMultiScale(gray_img, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(300, 300));
    waitKey(10);

    for (int i = 0; i < faces.size(); i++)
    {
      Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
      Point pt2(faces[i].x, faces[i].y);
      Mat faceROI = gray_img(faces[i]);
      rectangle(gray_img, pt1, pt2, cvScalar(0, 255, 0), 2, 8, 0);

      Rect reyeROI(pt2.x, pt2.y + faces[i].height / 3, faces[i].width / 2, faces[i].height / 6);
      Rect leyeROI((pt2.x + pt1.x) / 2, pt2.y + faces[i].height / 3, faces[i].width / 2, faces[i].height / 6);
      Rect mouthROI(pt2.x + (pt1.x - pt2.x) * 1 / 4, pt2.y + (pt1.y - pt2.y)*11.5 / 16, faces[i].width * 1 / 2, faces[i].height * 1 / 4);

      
      Mat get_leye = gray_img(leyeROI);
      Mat get_reye = gray_img(reyeROI);
      Mat get_mouth = gray_img(mouthROI);

      Mat crop_leye, crop_reye, crop_mouth;
      get_leye.copyTo(crop_leye);
      get_reye.copyTo(crop_reye);
      get_mouth.copyTo(crop_mouth);

      //imshow("left eye", crop_leye);
      //imshow("right eye", crop_reye);
      // Do training here
      
      for(int an = 0; an < 150; an+=30)
      {
        Mat kernel = GaborFilter::GaborKernel(6.0, an * CV_PI / 180, 33 * CV_PI / 180, 9.0, 4.0, 13);

        //cv::filter2D(gray_img, dest, CV_32F, kernel);
        Mat dest = Util::FilterMat2D(crop_leye, kernel);
        Mat dest1 = Util::FilterMat2D(crop_reye, kernel);
        Mat dest2 = Util::FilterMat2D(crop_mouth, kernel);

        float le = Util::LocalEnergy(dest, dest.cols, dest.rows);
        float le1 = Util::LocalEnergy(dest1, dest1.cols, dest1.rows);
        float le2 = Util::LocalEnergy(dest2, dest2.cols, dest2.rows);

        float ma = Util::MeanAmplitude(dest, dest.cols, dest.rows);
        float ma1 = Util::MeanAmplitude(dest1, dest1.cols, dest1.rows);
        float ma2 = Util::MeanAmplitude(dest2, dest2.cols, dest2.rows);

        //cout << " le-ang " << an << " " << le << " " << le1 << " " << le2 << endl;
        //cout << " ma-ang " << an << " " << ma << " " << ma1 << " " << ma2 << endl;
        le_list.push_back(std::to_string(le));
        le_list.push_back(std::to_string(le1));
        le_list.push_back(std::to_string(le2));
        ma_list.push_back(std::to_string(ma));
        ma_list.push_back(std::to_string(ma1));
        ma_list.push_back(std::to_string(ma2));

        // Free the memory
        kernel.release();
        dest.release();
        dest1.release();
        dest2.release();
      }


      std::string fv_le = "";
      std::string fv_ma = "";
      while (!le_list.empty())
      {
        fv_le += le_list.front() + ";";
        fv_ma += ma_list.front() + ";";
        le_list.pop_front();
        ma_list.pop_front();
      }

      if (!fv_ma.empty() && fv_ma[fv_ma.length()-1] == ';') {
        fv_ma.erase(fv_ma.length()-1);
      }

      cout << fv_le + fv_ma << endl; 

      feature_vec.push_back(fv_le + fv_ma);

      valid_file_list.push_back(current_file);

      le_list.clear();
      ma_list.clear();


      // Thresholding left eye
      for (int i = 0; i < crop_leye.rows; i++)
      {
        for (int j = 0; j < crop_leye.cols; j++)
        {
          if (crop_leye.at<uchar>(i, j) < alpha_slider)
          {
            crop_leye.at<uchar>(i, j) = 255;
          }
          else
          {
            crop_leye.at<uchar>(i, j) = 0;
          }
        }
      }
      // Thresholding right eye
      for (int i = 0; i < crop_reye.rows; i++)
      {
        for (int j = 0; j < crop_reye.cols; j++)
        {
          if (crop_reye.at<uchar>(i, j) < alpha_slider)
          {
            crop_reye.at<uchar>(i, j) = 255;
          }
          else
          {
            crop_reye.at<uchar>(i, j) = 0;
          }
        }
      }

      //Find Left Eyes
      int max_row = 0;
      int peak_row = 0;
      int peak_row_id = 0;
      int temp_sum_row = 0;

      for (int i = 0; i < crop_leye.rows; i++)
      {
        temp_sum_row = 0;
        for (int j = 0; j < crop_leye.cols - 20; j++)
        {
          uchar intensity = crop_leye.at<uchar>(i, j);
          temp_sum_row = temp_sum_row + intensity;

        }
        if (temp_sum_row > max_row)
        {
          peak_row = temp_sum_row;
          max_row = temp_sum_row;
          peak_row_id = i;
        }
      }

      int max_col = 0;
      int peak_col = 0;
      int peak_col_id = 0;
      int temp_sum_col = 0;

      for (int i = 0; i < crop_leye.cols - 20; i++)
      {
        temp_sum_col = 0;
        for (int j = 0; j < crop_leye.rows; j++)
        {
          uchar intensity = crop_leye.at<uchar>(j, i);
          temp_sum_col = temp_sum_col + intensity;
        }
        if (temp_sum_col > max_col)
        {
          peak_col = temp_sum_col;
          max_col = temp_sum_col;
          peak_col_id = i;
        }
      }

      //Find Right Eyes
      int max_row2 = 0;
      int peak_row2 = 0;
      int peak_row_id2 = 0;
      int temp_sum_row2 = 0;

      for (int i = 0; i < crop_reye.rows; i++)
      {
        temp_sum_row2 = 0;
        for (int j = 20; j < crop_reye.cols; j++)
        {
          uchar intensity = crop_reye.at<uchar>(i, j);
          temp_sum_row2 = temp_sum_row2 + intensity;

        }
        if (temp_sum_row2 > max_row2)
        {
          peak_row2 = temp_sum_row2;
          max_row2 = temp_sum_row2;
          peak_row_id2 = i;
        }
      }

      int max_col2 = 0;
      int peak_col2 = 0;
      int peak_col_id2 = 0;
      int temp_sum_col2 = 0;

      for (int i = 20; i < crop_reye.cols; i++)
      {
        temp_sum_col2 = 0;
        for (int j = 0; j < crop_reye.rows; j++)
        {
          uchar intensity = crop_reye.at<uchar>(j, i);
          temp_sum_col2 = temp_sum_col2 + intensity;
        }
        if (temp_sum_col2 > max_col2)
        {
          peak_col2 = temp_sum_col2;
          max_col2 = temp_sum_col2;
          peak_col_id2 = i;
        }
      }

      //Transform iris points to global coordinate
      int gray_img_cols = gray_img.cols;
      int half_face = faces[i].width / 2;
      
      int reye_y_dist, leye_y_dist;
      leye_y_dist = faces[i].x + half_face + peak_col_id;
      reye_y_dist = faces[i].x + peak_col_id2;

      int ED = leye_y_dist - reye_y_dist;
      int face_midPoint_y = reye_y_dist + ED / 2;
      int face_midPoint_x = faces[i].y + faces[i].height /2 * 2/3  + peak_row_id;

      int mouth_top = face_midPoint_x + (ED * 0.85);
      int mouth_bottom = mouth_top + (ED * 0.65);

      //imshow("mouth", crop_mouth);
    }

    in.release();
    gray_img.release();
    file_list.pop_front();
  }

  // Write into a file

  ofstream tdata;
  tdata.open("./training.csv");

  while (!feature_vec.empty())
  {
    tdata<< feature_vec.front() + "," + valid_file_list.front() + "\n";
    feature_vec.pop_front();
    valid_file_list.pop_front();
  }

  tdata.close();

  feature_vec.clear();
  valid_file_list.clear();

/*

      */
	return 0;
}
