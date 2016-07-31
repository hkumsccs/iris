#include "stdio.h"
#include "math.h"
#include "opencv/cv.h"
#include <opencv/highgui.h>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "util.cpp"

using namespace cv;
using namespace std;

#define blue  CV_RGB(0,0,255)
#define green CV_RGB(0,255,0)
#define red   CV_RGB(255,0,0)
#define white CV_RGB(255,255,255)
#define black CV_RGB(0,0,0)



int main()
{
	CascadeClassifier face_cascade;
	if(!face_cascade.load("./metadata/haarcascade_frontalface_alt.xml")) {
		printf("Error loading cascade file for face");
		return 1;
	}
	VideoCapture capture(0); //-1, 0, 1 device id
	if(!capture.isOpened())
	{
		printf("error to initialize camera");
		return 1;
	}
  capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);  
  capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);  
  
	Mat cap_img, gray_img;
  Mat crop_leye,crop_reye, crop_lBrow, crop_rBrow;
	vector<Rect> faces, eyes;
	while(1)
	{
		capture >> cap_img;
		waitKey(1);
		cvtColor(cap_img, gray_img, CV_BGR2GRAY);
		//cv::equalizeHist(gray_img,gray_img);
		face_cascade.detectMultiScale(gray_img, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0,0), cvSize(200,200));
		for(int i=0; i < faces.size();i++)
		{
			Point pt1(faces[i].x+faces[i].width, faces[i].y+faces[i].height);
			Point pt2(faces[i].x,faces[i].y);
			Mat faceROI = gray_img(faces[i]);
			rectangle(gray_img, pt1, pt2, cvScalar(0,255,0), 2, 8, 0);

      //line(gray_img, Point((pt1.x + pt2.x)/2,pt2.y), Point((pt1.x + pt2.x)/2,(pt1.y + pt2.y)/2), Scalar(255,130,106,255), 2);
      //line(gray_img, Point(pt2.x,(pt1.y+pt2.y)/2), Point(pt1.x,(pt1.y+pt2.y)/2), Scalar(255,130,106,255), 2);
	
      //Crop ROI
      //Rect leyeROI(faces[i].x, faces[i].y, faces[i].width / 2, faces[i].height / 2);
      //Rect reyeROI(faces[i].x + faces[i].width / 2, faces[i].x + faces[i].width / 2, faces[i].width / 2, faces[i].height / 2);

      //Rect lBrowROI(pt2.x, pt2.y + faces[i].height/6, faces[i].width/2, faces[i].height/6);
      //Rect rBrowROI((pt2.x+pt1.x)/2, pt2.y + faces[i].height/6, faces[i].width/2, faces[i].height/6);
	    Rect leyeROI(pt2.x + 15, pt2.y + faces[i].height/6, faces[i].width/2 - 15, faces[i].height/2);
      Rect reyeROI((pt2.x+pt1.x)/2, pt2.y + faces[i].height/6, faces[i].width/2 - 15, faces[i].height/2);

      //Rect leyeROI(pt2.x, pt2.y, faces[i].width/2, faces[i].height/2);
      //Rect reyeROI((pt2.x+pt1.x)/2, pt2.y, faces[i].width/2, faces[i].height/2);

      crop_leye = gray_img(leyeROI);
      crop_reye = gray_img(reyeROI);
	    //crop_lBrow = gray_img(lBrowROI);
	    //crop_rBrow = gray_img(rBrowROI);

	    //imshow("Cropped left eyeBrow", crop_lBrow);
	    //imshow("Cropped right eyeBrow", crop_rBrow);

      Mat cm_img0 = InvertedImage(crop_leye, crop_leye.cols, crop_leye.rows, 200);
      Mat cm_img1 = InvertedImage(crop_reye, crop_reye.cols, crop_reye.rows, 200);
      Util::Smoothing(cm_img0, 0.005 * faces[i].height, cm_img0.cols, cm_img0.rows);
      Util::Smoothing(cm_img1, 0.005 * faces[i].height, cm_img1.cols, cm_img1.rows);
      // Apply the colormap:
      //applyColorMap(cm_img0, cm_img0, COLORMAP_JET);
      //applyColorMap(cm_img1, cm_img1, COLORMAP_JET);
      
      // Locate the center of iris
      std::pair<int, int> left_iris = FindIris(cm_img0);
      std::pair<int, int> right_iris = FindIris(cm_img1);
 
      int left_iris_x = std::get<0>(left_iris);
      int left_iris_y = std::get<1>(left_iris);
      int right_iris_x = std::get<0>(right_iris);
      int right_iris_y = std::get<1>(right_iris);

      cout << left_iris_x << " left " << left_iris_y << endl;
      cout << right_iris_x << " right " << right_iris_y << endl;
      
      //Point r_center = Point(std::get<0>(right_iris), std::get<1>(right_iris));
      //circle(crop_reye, r_center, 2, CV_RGB(255,255,0), 3);
      
      //Point l_center = Point(std::get<0>(left_iris), std::get<1>(left_iris));
      //circle(crop_leye, l_center, 2, CV_RGB(0,0,0), 3);

      //imshow("Cropped left eye", GradientImage(crop_leye, crop_leye.cols, crop_leye.rows));
      //imshow("Cropped right eye", GradientImage(crop_reye, crop_reye.cols, crop_reye.rows));


      //line(cm_img0, Point(0, left_iris_y), Point(crop_leye.cols - 1, left_iris_y), Scalar(255,130,106,255), 1);
      //line(cm_img0, Point(left_iris_x, 0), Point(left_iris_x, crop_leye.rows - 1), Scalar(255,130,106,255), 1);

      //line(cm_img1, Point(0, right_iris_y), Point(crop_reye.cols - 1, right_iris_y), Scalar(255,130,106,255), 1);
      //line(cm_img1, Point(right_iris_x, 0), Point(right_iris_x, crop_reye.rows - 1), Scalar(255,130,106,255), 1);

	    //Rect leyeROI(pt2.x + 15, pt2.y + faces[i].height/4, faces[i].width/2 - 15, faces[i].height/6);
      int offset_x_1 = pt2.x + 15;
      int offset_y_1 = pt2.y + faces[i].height/6;
      //Rect reyeROI((pt2.x+pt1.x)/2, pt2.y + faces[i].height/4, faces[i].width/2 - 15, faces[i].height/6);
      int offset_x_2 = (pt2.x + pt1.x)/2;
      int offset_y_2 = pt2.y + faces[i].height/6;

      // Display the iris with respect to the entire image
      line(gray_img, Point(offset_x_1 + left_iris_x - 2, offset_y_1 + left_iris_y), Point(offset_x_1 + left_iris_x + 2, offset_y_1 + left_iris_y), Scalar(255,130,106,255), 1);
      line(gray_img, Point(offset_x_1 + left_iris_x, offset_y_1 + left_iris_y - 2), Point(offset_x_1 + left_iris_x, offset_y_1 + left_iris_y + 2), Scalar(255,130,106,255), 1);

      line(gray_img, Point(offset_x_2 + right_iris_x - 2, offset_y_2 + right_iris_y), Point(offset_x_2 + right_iris_x + 2, offset_y_2 + right_iris_y), Scalar(255,130,106,255), 1);
      line(gray_img, Point(offset_x_2 + right_iris_x, offset_y_2 + right_iris_y - 2), Point(offset_x_2 + right_iris_x, offset_y_2 + right_iris_y + 2), Scalar(255,130,106,255), 1);

      imshow("Cropped left eye", cm_img0);
      imshow("Cropped right eye", cm_img1);
		}

    imshow("Result", gray_img);
		char c = waitKey(3);
		if(c == 27)
			break;
	}
	return 0;
}
