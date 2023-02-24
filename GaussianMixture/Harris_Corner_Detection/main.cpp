#include <stdio.h>
#include <Windows.h>
#include <omp.h>
#include "OpenCV\InitOpenCV.h"
#include <vector>
#include <time.h>
using namespace std;
#define SAMPLEDATA 1000

#define WINDOW 3
#define GMMCOUNT WINDOW*WINDOW

struct GM
{
	float meany[GMMCOUNT];
	float meanx[GMMCOUNT];
	float varx[GMMCOUNT];
	float vary[GMMCOUNT];
	int sampledatacount[GMMCOUNT];
};

void Setinit(Mat& src)
{
	//Class1
	for (int i = 0; i < SAMPLEDATA; i++)
	{
		int x = rand() % 250 + 50;
		int y = rand() % 350 + 50;
		src.at<Vec3b>(y, x)[0] = 255;		
	}

	//Class2
	for (int i = 0; i < SAMPLEDATA; i++)
	{
		int x = rand() % 300 + 200;
		int y = rand() % 350 + 150;
		src.at<Vec3b>(y, x)[1] = 255;		
	}

	//Class3
	for (int i = 0; i < SAMPLEDATA; i++)
	{
		int x = rand() % 200 + 300;
		int y = rand() % 150 + 50;
		src.at<Vec3b>(y, x)[2] = 255;		
	}
}

GM GetGM(Mat& src, int id)
{
	int w = 500 / WINDOW;
	int h = 500 / WINDOW;
	GM gm1;

	for (int i = 0;i < GMMCOUNT;i++)
	{
		gm1.meanx[i] = 0;
		gm1.meany[i] = 0;
		gm1.sampledatacount[i] = 0;

		int tid = id;

		for (int dx = 0; dx < w;dx++)
		{
			for (int dy = 0; dy < w;dy++)
			{
				int rowx = i / WINDOW;
				int rowy = i % WINDOW;
				int x = dx + w * rowx;
				int y = dy + h * rowy;

				if (x >= 0 && x < 500 && y >= 0 && y < 500)
				{
					if (src.at<Vec3b>(y, x)[tid] == 255)
					{
						gm1.meanx[i] += x;
						gm1.meany[i] += y;
						gm1.sampledatacount[i]++;
					}
				}
			}
		}

		gm1.meanx[i] /= (float)gm1.sampledatacount[i];
		gm1.meany[i] /= (float)gm1.sampledatacount[i];

		gm1.varx[i] = 0;
		gm1.vary[i] = 0;

		for (int dx = 0;dx < w; dx++)
		{
			for (int dy = 0;dy < w; dy++)
			{
				int rowx = i / WINDOW;
				int rowy = i % WINDOW;
				int x = dx + w * rowx;
				int y = dy + h * rowy;

				if (x >= 0 && x < 500 && y >= 0 && y < 500)
				{
					if (src.at<Vec3b>(y, x)[tid] == 255)
					{
						float dtx = (gm1.meanx[i] - x) * (gm1.meanx[i] - x);
						float dty = (gm1.meany[i] - y) * (gm1.meany[i] - y);
						gm1.varx[i] += sqrt(dtx);
						gm1.vary[i] += sqrt(dtx);
					}
				}
			}
		}

		gm1.varx[i] /= (float)gm1.sampledatacount[i];
		gm1.vary[i] /= (float)gm1.sampledatacount[i];

		if (gm1.sampledatacount[i] == 0)
		{
			gm1.meanx[i] = 0;
			gm1.meany[i] = 0;
			gm1.varx[i] = 0;
			gm1.vary[i] = 0;
		}
	}
	return gm1;
}

double GetGMv(GM gm, int x, int y)
{
	double totalRate = 0;
	omp_set_dynamic(1);
#pragma dmp parallel num_threads(8)
	{
		int i;
#pragma dmp for private(i)
		for (i = 0; i < GMMCOUNT; i++)
		{
			float v = (gm.varx[i] * gm.vary[i]) + (gm.varx[i] + gm.vary[i]);
			double p1 = sqrt(2 * 3.14 * v);			
			double ppx = (x - gm.meanx[i]) * (x - gm.meanx[i]);
			double ppy = (y - gm.meany[i]) * (y - gm.meany[i]);

			double p2 = -(ppx + ppy) / (2 * v);

			double p3 = (1.0f / p1) * expf(p2);
			if (v == 0.0)
				p3 = 0;
			totalRate += p3;
		}
	}
	return totalRate;
}

float GetMax(GM gm)
{
	float fmax = 0;
	for (int i = 0; i < GMMCOUNT; i++)
	{
		float p = GetGMv(gm, gm.meanx[i], gm.meany[i]);
		if (p > fmax)
			fmax = p;
	}
	return fmax;
}


int main()
{
	srand(time(NULL));

	Mat src(500, 500, CV_8UC3, Scalar(0));
	Mat src1(500, 500, CV_8UC3, Scalar(0));
	Mat src2(500, 500, CV_8UC3, Scalar(0));
	Setinit(src);
	GM gm1 = GetGM(src, 0);
	GM gm2 = GetGM(src, 1);
	GM gm3 = GetGM(src, 2);

	float fmin[3];
	float fmax[3];
	float len[3];
	for (int i = 0; i < 3;i++)
	{
		fmin[i] = 0;
	}
	fmax[0] = GetMax(gm1);
	fmax[1] = GetMax(gm2);
	fmax[2] = GetMax(gm3);

	len[0] = fmax[0] - fmin[0];
	len[1] = fmax[1] - fmin[1];
	len[2] = fmax[2] - fmin[2];

	for (int x = 0;x < 500;x++)
	{
		for (int y = 0;y < 500;y++)
		{
			float p1 = GetGMv(gm1, x, y);
			float p2 = GetGMv(gm2, x, y);
			float p3 = GetGMv(gm3, x, y);

			float ll1 = (p1 - fmin[0]) / len[0] * 255.0f;
			float ll2 = (p2 - fmin[1]) / len[1] * 255.0f;
			float ll3 = (p3 - fmin[2]) / len[2] * 255.0f;

			src2.at<Vec3b>(y, x)[0] = ll1;
			src2.at<Vec3b>(y, x)[1] = ll2;
			src2.at<Vec3b>(y, x)[2] = ll3;

			float plen = p1 + p2 + p3;
			if (plen > 0)
			{
				p1 = (p1 / plen * 255.0f);
				p2 = (p2 / plen * 255.0f);
				p3 = (p3 / plen * 255.0f);
			}
			src1.at<Vec3b>(y, x)[0] = p1;
			src1.at<Vec3b>(y, x)[1] = p2;
			src1.at<Vec3b>(y, x)[2] = p3;
		}
	}

	imshow("img", src);
	imshow("img1", src1);
	imshow("img2", src2);
	cvWaitKey();
	return 0; 
}