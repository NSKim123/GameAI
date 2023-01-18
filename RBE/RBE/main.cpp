#include <stdio.h>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include "OpenCV\InitOpenCV.h"

float mouse_x;
float mouse_y;

void Mouse_CallBack(int nevent, int x, int y, int flags, void* userdata)
{
	mouse_x = (float)x;
	mouse_y = (float)y;
}

float onRBE(float x, float EstimationX, float beforeDir)
{
	float p1 = x - EstimationX;
	float p2 = (p1 * p1);
	float mean = (x + EstimationX) / 2.0f;
	float p3 = x - mean;
	float p4 = EstimationX - mean;
	float p5 = (p3 * p3) + (p4 * p4);
	float RBE = exp(-p2 / (p5 * p5));

	if (p2 == 0)
		RBE = 0;

	float dir = p1 * RBE;

	return beforeDir * 0.5 + dir * 0.5;
}

int main()
{
	mouse_x = 0;
	mouse_y = 0;


	Mat src(800, 800, CV_8UC3, Scalar(0)); 

	namedWindow("RBE", 1);
	setMouseCallback("RBE", Mouse_CallBack, NULL);

	float bx = 0;
	float by = 0;
	float bDirx = 0;
	float bDiry = 0;

	while (1)
	{
		bDirx = onRBE(mouse_x, bx, bDirx);
		bDiry = onRBE(mouse_y, by, bDiry);
		bx += bDirx;
		by += bDiry;


		circle(src, Point(mouse_x, mouse_y), 5, Scalar(0, 0, 255), -1);  //OpenCV는 BGR 순서이다. 
		circle(src, Point(bx,by), 5, Scalar(255, 0, 0), -1);  //OpenCV는 BGR 순서이다. 

		imshow("RBE", src);
		cvWaitKey(10);
	}


	return 0;
}