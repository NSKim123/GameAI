#include <stdio.h>
#include <Windows.h>
#include "OpenCV\InitOpenCV.h"

#define WINDOWSIZE 50
#define BIN 10

Mat onHistogram(Mat src, int id, int mx, int my)
{
	int nr = src.rows;
	int nc = src.cols;
	int max = 0;
	int min = 10000;
	int hin[255];

	int nbin = 255.0f / BIN;
	Mat result(100, 255.0f, CV_8UC3, Scalar(0));
	memset(hin, 0, sizeof(int) * 255);

	for (int y = (my - WINDOWSIZE / 2); y < (my + WINDOWSIZE / 2);y++)
	{
		for (int x = (mx - WINDOWSIZE / 2); x < (mx + WINDOWSIZE / 2);x++)
		{
			if (x >= 0 && x < nc && y>0 && y < nr)
			{
				int k = src.at<Vec3b>(y, x)[id];

				if (k >= max)
					max = k;
				if (k <= min)
					min = k;
			}
		}
	}
	float len = max - min;
	for (int y = (my - WINDOWSIZE / 2); y < (my + WINDOWSIZE / 2);y++)
	{
		for (int x = (mx - WINDOWSIZE / 2); x < (mx + WINDOWSIZE / 2);x++)
		{
			if (x >= 0 && x < nc && y>0 && y < nr)
			{
				int k = src.at<Vec3b>(y, x)[id];
				int plen = (k - min) / len * nbin;
				if (plen < 0)
					plen = 0;
				if (plen >= nbin)
					plen = nbin - 1;
				hin[plen]++;
			}
		}
	}

	float Maxi = WINDOWSIZE * WINDOWSIZE;

	for (int t = 0; t < 255;t++)
	{
		int i = t / BIN;
		if (i < nbin)
		{
			int slen = (float)hin[i] / Maxi * 99.0f;
			for (int j = 0; j < slen; j++)
			{
				result.at<Vec3b>(99 - j, t)[id] = 255;
			}
		}
	}

	return result;
}

void onMouseEvent(int evt, int x, int y, int flags, void* src)
{
	Mat img = *(Mat*)src;

	Mat Histograming[3];
	switch (evt)
	{
	case CV_EVENT_LBUTTONDOWN:
		Histograming[0] = onHistogram(img, 2, x, y);
		Histograming[1] = onHistogram(img, 1, x, y);
		Histograming[2] = onHistogram(img, 0, x, y);
		imshow("Histograming_R", Histograming[0]);
		imshow("Histograming_G", Histograming[1]);
		imshow("Histograming_B", Histograming[2]);

		break;
	}

	imshow("img", img);
}

int main()
{
	printf("WindowSize : %d X %d, BIN : %d \n", WINDOWSIZE, WINDOWSIZE, BIN);

	Mat img = imread("0.png");
	imshow("img", img);
	setMouseCallback("img", onMouseEvent, (void*)&img);
	cvWaitKey(0);
	img.release();

	return 0;
}

