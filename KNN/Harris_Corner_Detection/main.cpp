#include <stdio.h>
#include <Windows.h>
#include <omp.h>
#include "OpenCV\InitOpenCV.h"
#include <vector>
#include <time.h>
using namespace std;
#define SAMPLEDATA 100

#define K 50

struct SDATA
{
	float x;
	float y;
	int class_id;
	float len;
	bool buse;
};
struct KNNDATA
{
	float meanx;
	float meany;
	int class_id;
};

vector<KNNDATA> g_KNNList;
vector<SDATA> g_DATA_List;

void Setinit(Mat& src)
{
	//Class1
	for (int i = 0; i < SAMPLEDATA; i++)
	{
		int x = rand() % 250 + 50;
		int y = rand() % 350 + 50;
		src.at<Vec3b>(y, x)[0] = 255;
		SDATA a;
		a.x = x;
		a.y = y;
		a.class_id = 0;
		a.buse = false;
		g_DATA_List.push_back(a);
	}

	//Class2
	for (int i = 0; i < SAMPLEDATA; i++)
	{
		int x = rand() % 300 + 200;
		int y = rand() % 350 + 150;
		src.at<Vec3b>(y, x)[1] = 255;
		SDATA a;
		a.x = x;
		a.y = y;
		a.class_id = 1;
		a.buse = false;
		g_DATA_List.push_back(a);
	}

	//Class2
	for (int i = 0; i < SAMPLEDATA; i++)
	{
		int x = rand() % 200 + 300;
		int y = rand() % 150 + 50;
		src.at<Vec3b>(y, x)[2] = 255;
		SDATA a;
		a.x = x;
		a.y = y;
		a.class_id = 2;
		a.buse = false;
		g_DATA_List.push_back(a);
	}
}

bool comp(SDATA a, SDATA b)
{
	return (a.len < b.len);
}

void onTrainKNN()
{
	for (int i = 0; i < g_DATA_List.size(); i++)
	{
		if (g_DATA_List[i].buse == false)
		{
			for (int j = 0; j < g_DATA_List.size();j++)
			{
				if (g_DATA_List[j].buse == false)
				{
					float dx = g_DATA_List[i].x - g_DATA_List[j].x;
					float dy = g_DATA_List[i].y - g_DATA_List[j].y;
					g_DATA_List[j].len = (dx * dx) + (dy * dy);
				}
				else
					g_DATA_List[j].len = 10000000;
			}
			sort(g_DATA_List.begin(), g_DATA_List.end(), comp);
			KNNDATA a;
			a.meanx = 0;
			a.meany = 0;
			int count = 0;
			int Class_rate[3];
			memset(Class_rate, 0, sizeof(int) * 3);
			for (int j = 0; j < K;j++)
			{
				if (g_DATA_List[j].buse == false)
				{
					a.meanx += g_DATA_List[j].x;
					a.meany += g_DATA_List[j].y;
					count++;
					Class_rate[g_DATA_List[j].class_id]++;
					g_DATA_List[j].buse = true;
				}
			}
			if (count != 0)
			{
				a.meanx /= (float)count;
				a.meany /= (float)count;

				int nmax = 0;
				int id = 0;
				for (int j = 0;j < 3;j++)
				{
					if (Class_rate[j] >= nmax) 
					{
						id = j;
						nmax = Class_rate[j];
					}					
				}
				a.class_id = id;
				g_KNNList.push_back(a);
			}
		}
	}
}

int GetKNNID(float x, float y)
{
	float fmin = 1000000;
	int id = 0;
	for (int i = 0;i < g_KNNList.size();i++)
	{
		float dx = g_KNNList[i].meanx - x;
		float dy = g_KNNList[i].meany - y;

		float len = (dx * dx) + (dy * dy);
		if (fmin >= len)
		{
			fmin = len;
			id = g_KNNList[i].class_id;
		}
	}
	return id;
}
int main()
{
	g_DATA_List.clear();
	g_KNNList.clear();
	srand(time(NULL));

	Mat src(500, 500, CV_8UC3, Scalar(0));
	Mat src1(500, 500, CV_8UC3, Scalar(0));
	Setinit(src);
	onTrainKNN();

	for (int x = 0; x < 500;x++)
	{
		for (int y = 0;y < 500;y++)
		{
			int id = GetKNNID(x, y);
			src1.at<Vec3b>(y, x)[id] = 255;
		}
	}

	imshow("img", src);
	imshow("img1", src1);
	cvWaitKey();
	return 0; 
}