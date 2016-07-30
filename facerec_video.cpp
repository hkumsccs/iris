#include "stdio.h"
#include "math.h"
#include "opencv/cv.h"
#include <opencv/highgui.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

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
	if(!face_cascade.load("./metadata/lbpcascade_frontalface.xml")) {
		printf("Error loading cascade file for face");
		return 1;
	}
	VideoCapture capture(0); //-1, 0, 1 device id
	if(!capture.isOpened())
	{
		printf("error to initialize camera");
		return 1;
	}
	Mat cap_img,gray_img;
	vector<Rect> faces, eyes;
	while(1)
	{
		capture >> cap_img;
		waitKey(10);
		cvtColor(cap_img, gray_img, CV_BGR2GRAY);
		cv::equalizeHist(gray_img,gray_img);
		face_cascade.detectMultiScale(gray_img, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0,0), cvSize(300,300));
		for(int i=0; i < faces.size();i++)
		{
			Point pt1(faces[i].x+faces[i].width, faces[i].y+faces[i].height);
			Point pt2(faces[i].x,faces[i].y);
			Mat faceROI = gray_img(faces[i]);
			rectangle(gray_img, pt1, pt2, cvScalar(0,255,0), 2, 8, 0);
      //line(gray_img, Point((pt2.x - pt1.x)/2, pt1.y), Point((pt2.x - pt1.x)/2, (pt2.y-pt1.y)/2), cvScalar(0,255,0));
      line(gray_img, Point((pt1.x + pt2.x)/2,pt2.y), Point((pt1.x + pt2.x)/2,(pt1.y + pt2.y)/2), Scalar(255,130,106,255), 2);
      line(gray_img, Point(pt2.x,(pt1.y+pt2.y)/2), Point(pt1.x,(pt1.y+pt2.y)/2), Scalar(255,130,106,255), 2);
		
		//Crop ROI
		Rect leyeROI(faces[i].x, faces[i].y, faces[i].width / 2, faces[i].height / 2);
		Rect reyeROI(faces[i].x + faces[i].width / 2, faces[i].x + faces[i].width / 2, faces[i].width / 2, faces[i].height / 2);
		Mat crop_leye = gray_img(leyeROI);
		Mat crop_reye = gray_img(reyeROI);
		//imshow("left eye", crop_leye);
		//imshow("right eye", crop_reye);	
		
		}
		imshow("Result", gray_img);
		waitKey(3);
		char c = waitKey(3);
		if(c == 27)
			break;
	}
	return 0;
}
