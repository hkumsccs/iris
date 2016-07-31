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
	if (!face_cascade.load("haarcascade_frontalface_alt.xml")) {
		printf("Error loading cascade file for face");
		return 1;
	}
	VideoCapture capture(0); //-1, 0, 1 device id
	if (!capture.isOpened())
	{
		printf("error to initialize camera");
		return 1;
	}
	Mat cap_img, gray_img;
	vector<Rect> faces, eyes;
	while (1)
	{
		capture >> cap_img;
		waitKey(10);
		cvtColor(cap_img, gray_img, CV_BGR2GRAY);
		cv::equalizeHist(gray_img, gray_img);
		face_cascade.detectMultiScale(gray_img, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(300, 300));
		for (int i = 0; i < faces.size(); i++)
		{
			Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
			Point pt2(faces[i].x, faces[i].y);
			Mat faceROI = gray_img(faces[i]);
			rectangle(gray_img, pt1, pt2, cvScalar(0, 255, 0), 2, 8, 0);
			//line(gray_img, Point((pt2.x - pt1.x)/2, pt1.y), Point((pt2.x - pt1.x)/2, (pt2.y-pt1.y)/2), cvScalar(0,255,0));
			line(gray_img, Point((pt1.x + pt2.x) / 2, pt2.y), Point((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2), Scalar(255, 130, 106, 255), 2);
			line(gray_img, Point(pt2.x, (pt1.y + pt2.y) / 2), Point(pt1.x, (pt1.y + pt2.y) / 2), Scalar(255, 130, 106, 255), 2);

			//Crop ROI
			//Rect leyeROI(pt2.x, pt2.y, faces[i].width / 2, faces[i].height / 2);
			//Rect reyeROI((pt2.x + pt1.x) / 2, pt2.y, faces[i].width / 2, faces[i].height / 2);

			Rect leyeROI(pt2.x, pt2.y + faces[i].height / 3, faces[i].width / 2, faces[i].height / 6);
			Rect reyeROI((pt2.x + pt1.x) / 2, pt2.y + faces[i].height / 3, faces[i].width / 2, faces[i].height / 6);

			Mat get_leye = gray_img(leyeROI);
			Mat get_reye = gray_img(reyeROI);

			Mat crop_leye,crop_reye;
			get_leye.copyTo(crop_leye);
			get_reye.copyTo(crop_reye);

			//imshow("Cropped left", crop_leye);
			//imshow("Cropped right", crop_reye);

			//crop_reye = gray_img(reyeROI);

			//Threshholding
			for (int i = 0; i < crop_leye.rows; i++)
			{
				for (int j = 0; j < crop_leye.cols; j++)
				{
					if (crop_leye.at<uchar>(i, j) < 20)
					{
						crop_leye.at<uchar>(i, j) = 255;
					}
					else
					{
						crop_leye.at<uchar>(i, j) = 0;
					}
				}
			}

			for (int i = 0; i < crop_reye.rows; i++)
			{
				for (int j = 0; j < crop_reye.cols; j++)
				{
					if (crop_reye.at<uchar>(i, j) < 20)
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
			int peak_row_id = NULL;
			int temp_sum_row = 0;

			//uchar intensity = crop_leye.at<uchar>(15, 45);

			for (int i = 0; i < crop_leye.rows; i++)
			{
				temp_sum_row = 0;
				for (int j = 0; j < crop_leye.cols; j++)
				{
					uchar intensity = crop_leye.at<uchar>(i, j);
					temp_sum_row = temp_sum_row + intensity;

				}
				//printf("(%d ", temp_sum_row);
				if (temp_sum_row > max_row)
				{
					peak_row = temp_sum_row;
					max_row = temp_sum_row;
					peak_row_id = i;
					//printf("(%d,%d)", peak_row_id, peak_row);

				}
			}
			
			int max_col = 0;
			int peak_col = 0;
			int peak_col_id = NULL;
			int temp_sum_col = 0;

			//uchar intensity = crop_leye.at<uchar>(0, 10);
			//printf("(%d,%d, intensity value: %d)", crop_leye.rows, crop_leye.cols,intensity);
			//printf("(%d,%d, intensity value: %d)", crop_leye.rows, crop_leye.cols,intensity);

			for (int i = 10; i < crop_leye.cols; i++)
			{
				temp_sum_col = 0;
				for (int j = 0; j < crop_leye.rows; j++)
				{
					uchar intensity = crop_leye.at<uchar>(j, i);
					temp_sum_col = temp_sum_col + intensity;
				}
				//printf("(%d ", temp_sum_row);
				if (temp_sum_col > max_col)
				{
					peak_col = temp_sum_col;
					max_col = temp_sum_col;
					peak_col_id = i;
					printf("(%d,%d)", peak_col_id, peak_col);

				}
			}

			//Find Right Eyes
			int max_row2 = 0;
			int peak_row2 = 0;
			int peak_row_id2 = NULL;
			int temp_sum_row2 = 0;

			//uchar intensity = crop_leye.at<uchar>(15, 45);

			for (int i = 0; i < crop_reye.rows; i++)
			{
				temp_sum_row2 = 0;
				for (int j = 0; j < crop_reye.cols; j++)
				{
					uchar intensity = crop_reye.at<uchar>(i, j);
					temp_sum_row2 = temp_sum_row2 + intensity;

				}
				//printf("(%d ", temp_sum_row);
				if (temp_sum_row2 > max_row2)
				{
					peak_row2 = temp_sum_row2;
					max_row2 = temp_sum_row2;
					peak_row_id2 = i;
					//printf("(%d,%d)", peak_row_id, peak_row);

				}
			}

			int max_col2 = 0;
			int peak_col2 = 0;
			int peak_col_id2 = NULL;
			int temp_sum_col2 = 0;

			//uchar intensity = crop_leye.at<uchar>(0, 10);
			//printf("(%d,%d, intensity value: %d)", crop_leye.rows, crop_leye.cols,intensity);
			//printf("(%d,%d, intensity value: %d)", crop_leye.rows, crop_leye.cols,intensity);

			for (int i = 10; i < crop_reye.cols; i++)
			{
				temp_sum_col2 = 0;
				for (int j = 0; j < crop_reye.rows; j++)
				{
					uchar intensity = crop_reye.at<uchar>(j, i);
					temp_sum_col2 = temp_sum_col2 + intensity;
				}
				//printf("(%d ", temp_sum_row);
				if (temp_sum_col2 > max_col2)
				{
					peak_col2 = temp_sum_col2;
					max_col2 = temp_sum_col2;
					peak_col_id2 = i;
					printf("(%d,%d)", peak_col_id2, peak_col2);

				}
			}


			line(crop_leye, Point(0, peak_row_id), Point(crop_leye.cols, peak_row_id), Scalar(255, 255, 255), 2);
			line(crop_leye, Point(peak_col_id,0), Point(peak_col_id,crop_leye.rows), Scalar(255, 255, 255), 2);
			line(crop_reye, Point(0, peak_row_id2), Point(crop_reye.cols, peak_row_id2), Scalar(255, 255, 255), 2);
			line(crop_reye, Point(peak_col_id2, 0), Point(peak_col_id2, crop_reye.rows), Scalar(255, 255, 255), 2);


			imshow("left eye", crop_leye);
			imshow("right eye", crop_reye);

		}
		imshow("Result", gray_img);
		waitKey(3);
		char c = waitKey(3);
		if (c == 27)
			break;
	}
	return 0;
}
