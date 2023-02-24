#include <stdio.h>
#include "OpenCV\InitOpenCV.h"
#include "vector.h"

#include <time.h>


#define SAMPLEDATA 20

void Setinit(Mat& src)
{
	//Class 1
	for (int i = 0;i < SAMPLEDATA; i++)
	{
		int x = rand() % 250 + 50;
		int y = rand() % 350 + 50;
		src.at<Vec3b>(y, x)[0] = 255;
	}

	//Class 2
	for (int i = 0;i < SAMPLEDATA; i++)
	{
		int x = rand() % 300 + 200;
		int y = rand() % 350 + 150;
		src.at<Vec3b>(y, x)[1] = 255;
	}

	//Class 3
	for (int i = 0;i < SAMPLEDATA; i++)
	{
		int x = rand() % 200 + 300;
		int y = rand() % 150 + 50;
		src.at<Vec3b>(y, x)[2] = 255;
	}
}



int main()
{
	srand(time(NULL));

	Mat src(500, 500, CV_8UC3, Scalar(0));
	Mat src1(500, 500, CV_8UC3, Scalar(0));
	Mat src2(500, 500, CV_8UC3, Scalar(0));
	Setinit(src);

	float count[3] = { 0, };
	float mx[3] = { 0, };
	float my[3] = { 0, };

	for (int x = 0; x < 500;x++)
	{
		for (int y = 0; y < 500;y++)
		{
			int b = src.at<Vec3b>(y, x)[0];
			int g = src.at<Vec3b>(y, x)[1];
			int r = src.at<Vec3b>(y, x)[2];

			if (r != 0)
			{
				mx[0] += x;
				my[0] += y;
				count[0]++;
			}
			if (g != 0)
			{
				mx[1] += x;
				my[1] += y;
				count[1]++;
			}
			if (b != 0)
			{
				mx[2] += x;
				my[2] += y;
				count[2]++;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		mx[i] /= count[i];
		my[i] /= count[i];

		if (count[i] == 0)
		{
			mx[i] = -1000;
			my[i] = -1000;
		}		
	}
	float totI = count[0] + count[1] + count[2];

	for (int x = 0;x < 500;x++)
	{
		for (int y = 0;y < 500;y++)
		{
			float p[3] = { 0, };
			for (int i = 0; i < 3;i++)
			{
				if (mx[i] > 0)
					p[i] = 1.0f - (sqrt(((x - mx[i]) * (x - mx[i])) + ((y - my[i]) * (y - my[i]))) / 708.0f); //P(x | Class)
				p[i] *= count[i] / totI;
			}

			if (p[0] >= p[1] && p[0] >= p[2])
				src2.at<Vec3b>(y, x)[2] = 255;
			if (p[1] >= p[0] && p[1] >= p[2])
				src2.at<Vec3b>(y, x)[1] = 255;
			if (p[2] >= p[1] && p[2] >= p[0])
				src2.at<Vec3b>(y, x)[0] = 255;

			src1.at<Vec3b>(y, x)[2] = p[0] * 255;
			src1.at<Vec3b>(y, x)[1] = p[1] * 255;
			src1.at<Vec3b>(y, x)[0] = p[2] * 255;
		}
	}

	imshow("img", src);
	imshow("img1", src1);
	imshow("img2", src2);
	cvWaitKey();
	return 0; 
}