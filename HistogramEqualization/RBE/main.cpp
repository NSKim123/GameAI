#include <stdio.h>
#include <Windows.h>
#include "OpenCV\InitOpenCV.h"

void onHistogram(Mat& src)
{
	int nr = src.rows;
	int nc = src.cols;

	int max = 0;
	int min = 10000;

	for (int i = 0;i < (nr); i++)
	{
		for (int j = 0;j < nc; j++)
		{
			int k = (src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i, j)[2])/3;

			if (k >= max)
				max = k;
			if (k <= min)
				min = k;
		}
	}
	float len = max - min;

	for (int i = 0;i < (nr); i++)
	{
		for (int j = 0;j < nc; j++)
		{
			int k = (src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i, j)[2]) / 3;
			float plen = (k - min) / len * 255.0f;
			src.at<Vec3b>(i, j)[0] = plen;
			src.at<Vec3b>(i, j)[1] = plen;
			src.at<Vec3b>(i, j)[2] = plen;
		}
	}
}

int main()
{
	
	Mat img = imread("1.png");
	imshow("img", img);
	onHistogram(img);
	imshow("img1", img);
		
	cvWaitKey(10000000);
	img.release();
	
	return 0;
}