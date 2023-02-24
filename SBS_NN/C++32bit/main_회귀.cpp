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

#define INPUTDATA 100
#define PI 3.1415f

float input_data[INPUTDATA];
float correct_data[INPUTDATA];

void Init_dataSet()
{
	for (int i = 0; i < INPUTDATA; i++)
	{
		input_data[i] = ((float)i / (float)INPUTDATA * PI * 2.0f);
		correct_data[i] = sin(input_data[i]);
	}
}


void DrawAxis(Mat& src)
{
	src = Scalar(255, 255, 255);
	for (int i = 0; i < (INPUTDATA - 1); i++)
	{
		line(src, Point((input_data[i] / (PI * 2.0)) * 500, (correct_data[i] * 250) + 250), Point((input_data[i + 1] / (PI * 2.0)) * 500, (correct_data[i + 1] * 250) + 250), Scalar(0, 0, 0), 1);
	}

}

int main()
{
	srand((int)time(NULL));
	Init_dataSet();
	Mat src(500, 500, CV_8UC3, Scalar(0));


	//-- 각 설정 값 --
	int n_in = 1;  // 입력층 뉴런 수
	int n_mid = 50;  // 은닉층 뉴런 수
	int n_out = 1;  // 출력층 뉴런 수
	int interval = 10;  // 경과 표시 간격


	GuNeuralNetwork		guModel;
	guModel.AddLayer(INPUT_LAYER, n_in, RELU);
	guModel.AddLayer(OUTPUT_LAYER, n_mid, RELU);
	guModel.AddLayer(OUTPUT_LAYER, n_out, NONE_ACTIVE);
	guModel.Build();

	int frame = 0;
	while (1)
	{
		DrawAxis(src);

		float total_error = 0;
		for (int i = 0; i < INPUTDATA; i++)
		{

			float* x = (float*)&input_data[i];
			float y[3];
			y[0] = correct_data[i];

			guModel.Fit(x, y);

			if (frame % interval == 0)
			{
				float* prey = guModel.Predict(x);
				for (int j = 0; j < guModel.Get_Output_Vector_Size(); j++)
				{
					total_error += 1.0 / 2.0 * ((prey[j] - y[j]) * (prey[j] - y[j]));
				}

				circle(src, Point((x[0] / (PI * 2.0)) * 500, (prey[0] * 250) + 250), 3, Scalar(0, 255, 0), -1);
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


	guModel.Release();


	return 0;
}