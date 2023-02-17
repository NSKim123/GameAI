#include <stdio.h>
#include "OpenCV\InitOpenCV.h"
#include "vector.h"
#include <vector>
#include <time.h>
using namespace std;

void onGray(Mat& p1)
{
	for (int x = 0;x < p1.cols; x++)
	{
		for (int y = 0;y < p1.rows;y++)
		{
			float b = p1.at<Vec3b>(y, x)[0];
			float g = p1.at<Vec3b>(y, x)[1];
			float r = p1.at<Vec3b>(y, x)[2];

			float t = (b + g + r) / 3.0f;

			p1.at<Vec3b>(y, x)[0] = t;
			p1.at<Vec3b>(y, x)[1] = t;
			p1.at<Vec3b>(y, x)[2] = t;
		}
	}
}

void onHarris(Mat& p1, int KernelSize, float flat)
{
	int windowsize = KernelSize / 2;
	onGray(p1);
	GaussianBlur(p1, p1, Size(3, 3), 0, 0);
	Mat p2 = Mat::zeros(p1.rows, p1.cols, CV_8UC3);

	for (int hx = 0;hx <= p1.cols;hx += KernelSize)
	{
		for (int hy = 0;hy <= p1.rows;hy += KernelSize)
		{
			float A = 0;
			float B = 0;
			float C = 0;

			for (int tx = -windowsize; tx <= windowsize; tx++)
			{
				for (int ty = -windowsize; ty <= windowsize; ty++)
				{
					int x = hx + tx;
					int y = hy + ty;

					if (x >= 1 && x < p1.cols - 1 && y >= 1 && y < p1.rows - 1)
					{
						float dx = (p1.at<Vec3b>(y, x + 1)[0] - p1.at<Vec3b>(y, x - 1)[0])/255.0f;
						float dy = (p1.at<Vec3b>(y+1, x)[0] - p1.at<Vec3b>(y-1, x)[0])/255.0f;

						A += dx * dx;
						B += dy * dy;
						C += dx * dy;
					}
				}
			}

			float k = 0.04;
			float R = (A * B - C * C) - (k * (A + B) * (A + B));

			if (abs(R) >= flat)
			{
				if (R < 0)
				{
					p2.at<Vec3b>(hy, hx)[0] = 0;
					p2.at<Vec3b>(hy, hx)[1] = 255;
					p2.at<Vec3b>(hy, hx)[2] = 0;
				}
				else
				{
					p2.at<Vec3b>(hy, hx)[0] = 0;
					p2.at<Vec3b>(hy, hx)[1] = 0;
					p2.at<Vec3b>(hy, hx)[2] = 255;
				}
			}
		}
	}
	p1.release();
	p1 = p2.clone();
	p2.release();
}

int main()
{
	Mat img = imread("map.png");
	imshow("img",img);

	Mat gray = img.clone();
	onGray(gray);
	imshow("Gray",gray);

	Mat Cornelimg = img.clone();
	onHarris(Cornelimg,5,0.1);
	imshow("Cornelimg",Cornelimg);

	cvWaitKey();
	return 0; 
}