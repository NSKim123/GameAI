#include <stdio.h>
#include <Windows.h>
#include "OpenCV\InitOpenCV.h"

void onGray(Mat& img)
{
	for (int x = 0; x < img.cols; x++)
	{
		for (int y = 0;  y < img.rows ; y++)
		{
			float p = img.at<Vec3b>(y, x)[0] + img.at<Vec3b>(y, x)[1] + img.at<Vec3b>(y, x)[2];
			p /= 3.0f;
			img.at<Vec3b>(y, x)[0] = p;
			img.at<Vec3b>(y, x)[1] = p;
			img.at<Vec3b>(y, x)[2] = p;
		}
	}
}

void onDivX(Mat& img)
{
	for (int x = 1; x < img.cols - 1; x++)
	{
		for (int y = 0; y < img.rows; y++)
		{
			float p = abs(img.at<Vec3b>(y, x + 1)[0] - img.at<Vec3b>(y, x-1)[0])/2.0f;
			img.at<Vec3b>(y, x)[0] = p;
			img.at<Vec3b>(y, x)[1] = p;
			img.at<Vec3b>(y, x)[2] = p;
		}
	}
}

Mat onCanny(Mat& img, float Thr1, float Thr2)
{
	Mat tMap = img.clone();
	int k = 1;
	int ker = k * 2 + 1;

	for (int x = k;x < (img.cols - k);x++)
	{
		for (int y = k; y < (img.rows - k);y++)
		{
			tMap.at<Vec3b>(y, x)[0] = 0;
			tMap.at<Vec3b>(y, x)[2] = 0;
			tMap.at<Vec3b>(y, x)[1] = 0;

			float Gx = 0;
			float Gy = 0;
			for (int dx = -k; dx <= k;dx++)
			{
				for (int dy = -k; dy <= k;dy++)
				{
					int xx = dx + x;
					int yy = dy + y;

					float t = dx;
					if (dy == 0)
						t *= 2;
					
					if (dy == 1)
						t *= -1;

					float t1 = dy * -1;
					if (dx == 0)
						t1 *= 2;

					Gx += img.at<Vec3b>(yy, xx)[0] * t;
					Gy += img.at<Vec3b>(yy, xx)[0] * t1;
					
				}
			}

			float IG = sqrt((Gx * Gx) + (Gy * Gy));
			if (IG > Thr1 && IG < Thr2)
			{
				tMap.at<Vec3b>(y, x)[0] = 150;
				tMap.at<Vec3b>(y, x)[2] = 150;
				tMap.at<Vec3b>(y, x)[1] = 150;
			}
			else if (IG >= Thr2)
			{
				tMap.at<Vec3b>(y, x)[0] = 255;
				tMap.at<Vec3b>(y, x)[2] = 255;
				tMap.at<Vec3b>(y, x)[1] = 255;
			}
		}
	}
	return tMap;
}

Mat onMeanFilter(Mat& img, float Thr1)
{
	Mat tMap = img.clone();
	int k = Thr1;
	int ker = k * 2 + 1;

	for (int x = k;x < (img.cols - k);x++)
	{
		for (int y = k; y < (img.rows - k);y++)
		{				
			float result = 0.0f;
			
			for (int dx = -k; dx <= k;dx++)
			{
				for (int dy = -k; dy <= k;dy++)
				{
					int xx = dx + x;
					int yy = dy + y;

					result += tMap.at<Vec3b>(yy, xx)[0];					

				}
			}
			result /= (ker * ker);
			
			tMap.at<Vec3b>(y, x)[0] = result;
			tMap.at<Vec3b>(y, x)[1] = result;
			tMap.at<Vec3b>(y, x)[2] = result;
		}
	}
	return tMap;
}

Mat onMeanFilter1(Mat& img, float Thr1)
{
	Mat tMap = img.clone();
	int k = Thr1;
	int ker = k * 2 + 1;

	for (int x = k;x < (img.cols - k);x = x +ker)
	{
		for (int y = k; y < (img.rows - k);y = y +ker)
		{
			float result = 0.0f;

			for (int dx = -k; dx <= k;dx++)
			{
				for (int dy = -k; dy <= k;dy++)
				{
					int xx = dx + x;
					int yy = dy + y;

					result += tMap.at<Vec3b>(yy, xx)[0];

				}
			}
			result /= (ker * ker);

			for (int dx = -k; dx <= k;dx++)
			{
				for (int dy = -k; dy <= k;dy++)
				{
					int xx = dx + x;
					int yy = dy + y;

					tMap.at<Vec3b>(yy, xx)[0] = result;
					tMap.at<Vec3b>(yy, xx)[1] = result;
					tMap.at<Vec3b>(yy, xx)[2] = result;
				}
			}
			
		}
	}
	return tMap;
}

int main()
{
	
	Mat img = imread("0.png");
	onGray(img);
	//Mat t = onCanny(img, 180, 200);
	Mat t = onMeanFilter1(img, 3);
	/*imshow("img", img);
	imshow("img", t-img);*/
	imshow("img", t);
	cvWaitKey(10000000);
	


	return 0;
}