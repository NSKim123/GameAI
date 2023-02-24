#include <stdio.h>
#include <Windows.h>
#include<ctime>
#include "OpenCV\InitOpenCV.h"
#include "SBS_NN.h"	
struct Vector2f
{
	float x;
	float y;
	Vector2f() {}
	Vector2f(float Tx, float Ty)
	{
		x = Tx;
		y = Ty;
	}
};

#define INPUTDATA 20

float X[INPUTDATA];
float Y[INPUTDATA];
Vector2f input_data[INPUTDATA * INPUTDATA];
Vector2f correct_data[INPUTDATA * INPUTDATA];

void Init_dataSet()
{
	for (int i = 0; i < INPUTDATA; i++)
	{
		X[i] = ((float)i / (float)INPUTDATA * 2.0) - 1.0f;
		Y[i] = ((float)i / (float)INPUTDATA * 2.0) - 1.0f;
	}

	int id = 0;
	for (int i = 0; i < INPUTDATA; i++)
	{
		for (int j = 0; j < INPUTDATA; j++)
		{
			input_data[id] = Vector2f(X[i], Y[j]);
			float t = sin(X[i] * 3.1415);
			if (Y[j] < t)
				correct_data[id] = Vector2f(0, 1);
			else
				correct_data[id] = Vector2f(1, 0);
			id++;
		}
	}
}

void DrawAxis(Mat& src)
{
	src = Scalar(255, 255, 255);
	for (int i = 0; i < (INPUTDATA - 1); i++)
	{
		float t = sin(X[i] * 3.1415);
		float t1 = sin(X[i + 1] * 3.1415);
		line(src, Point((X[i] * 250) + 250, (t * 250) + 250), Point((X[i + 1] * 250) + 250, (t1 * 250) + 250), Scalar(0, 0, 0), 1);
	}
}
int main()
{
	srand((int)time(NULL));
	Init_dataSet();
	Mat src(500, 500, CV_8UC3, Scalar(0));

	//-- 각 설정 값 --
	int n_in = 2;  // 입력층 뉴런 수
	int n_mid = 12;  // 은닉층 뉴런 수
	int n_mid1 = 25;  // 은닉층 뉴런 수
	int n_mid2 = 6;  // 은닉층 뉴런 수
	int n_out = 2;  // 출력층 뉴런 수

	int interval = 10;  // 경과 표시 간격


	SBSNeuralNetwork		SBSModel;

	SBSModel.AddLayer(INPUT_LAYER, n_in, RELU);
	SBSModel.AddLayer(DENSE_LAYER, n_mid, RELU);
	SBSModel.AddLayer(DENSE_LAYER, n_mid1, RELU);
	SBSModel.AddLayer(DENSE_LAYER, n_mid2, RELU);
	SBSModel.AddLayer(OUTPUT_LAYER, n_out, SOFTMAX);
	SBSModel.Build();

	int frame = 0;
	while (1)
	{
		DrawAxis(src);

		float total_error = 0;
		for (int i = 0; i < (INPUTDATA * INPUTDATA); i++)
		{
			float* x = (float*)&input_data[i];
			float* y = (float*)&correct_data[i];
			SBSModel.Fit(x, y);


			if (frame % interval == 0)
			{
				float* prey = SBSModel.Predict(x);

				float local_error = 0;
				for (int j = 0; j < SBSModel.Get_Output_Vector_Size(); j++)
				{
					local_error += -(y[j] * log(prey[j] + 1e-7));
				}
				total_error += local_error;


				if (prey[0] > prey[1])
					circle(src, Point((x[0] * 250) + 250, (x[1] * 250) + 250), 5, Scalar(0, 0, 255), -1);
				else
					circle(src, Point((x[0] * 250) + 250, (x[1] * 250) + 250), 5, Scalar(0, 255, 0), -1);
			}
		}
		if (frame % interval == 0)
		{
			printf("total_error %f \n", total_error / (float)(INPUTDATA * INPUTDATA));
			imshow("GuNN", src);
			cvWaitKey(1);
		}
		frame++;
	}
	SBSModel.Release();
	return 0;
}
