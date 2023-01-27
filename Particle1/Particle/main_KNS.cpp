#include <stdio.h>
#include "OpenCV\InitOpenCV.h"
#include "vector.h"
#include <vector>

using namespace std;

//struct PARTICLE
//{
//	Vector2f Pos;
//	Vector2f Dir;
//	float Speed;
//	bool life;
//};
//
//Vector2f Goal = Vector2f(0, 0);
//void CallBackFunc(int event, int x, int y, int flags, void* userdata)
//{
//	if (event == EVENT_LBUTTONDOWN)
//	{
//		Goal = Vector2f(x, y);
//	}
//}
//
//
//int main()
//{
//	vector<PARTICLE> m_Particle;
//	m_Particle.clear();
//
//	for (int i = 0; i < 1000; i++)
//	{
//		PARTICLE a;
//		a.Pos = Vector2f(0, 0);
//		float r = (rand() % 360) / 180.0f * 3.14;
//		a.Dir = Vector2f(sin(r), cos(r));
//		a.Speed = rand() % 5 + 1;
//		a.life = true;
//		m_Particle.push_back(a);
//	}
//
//	
//	float count = 0;
//	Mat map = imread("map.png");
//	namedWindow("Particle", 1);
//	setMouseCallback("Particle", CallBackFunc, NULL);
//
//	/*for (int x = 0; x < map.cols; x += 1)
//	{
//		for (int y = 0; y < map.rows; y += 1)
//		{
//			int b = map.at<Vec3b>(y, x)[0];
//			int g = map.at<Vec3b>(y, x)[1];
//			int r = map.at<Vec3b>(y, x)[2];
//			if (r > g && r > b)
//			{
//				Goal += Vector2f(x, y);
//				count++;
//			}
//		}
//	}
//	Goal /= count;*/
//
//	int time = 0;
//	bool IsGoal = false;
//	/*Vector2f Response = Vector2f(0,0);
//    float AliveCount = 0;*/
//	while (1)
//	{
//		Mat tmap = map.clone();
//
//		float lenMax = 1000000;
//		int id = 0;
//
//		for (int i = 0; i < m_Particle.size(); i++)
//		{
//			if (time % 50 == 0) 
//			{
//				float r = (rand() % 360) / 180.0f * 3.14;
//				m_Particle[i].Dir = Vector2f(sin(r), cos(r));
//				m_Particle[i].Speed = rand() % 5 + 1;
//			}
//			m_Particle[i].Pos += m_Particle[i].Dir * m_Particle[i].Speed;
//			Vector2f pt = m_Particle[i].Pos - Goal;
//			if (pt.Length() <= 10)
//			{
//				IsGoal = true;
//			}
//
//			if (m_Particle[i].Pos.x >= 0 && m_Particle[i].Pos.x < 600 && m_Particle[i].Pos.y >= 0 && m_Particle[i].Pos.y < 600)
//			{
//				int b = map.at<Vec3b>(m_Particle[i].Pos.y, m_Particle[i].Pos.x)[0];
//				int g = map.at<Vec3b>(m_Particle[i].Pos.y, m_Particle[i].Pos.x)[1];
//				int r = map.at<Vec3b>(m_Particle[i].Pos.y, m_Particle[i].Pos.x)[2];
//				int p = (b + g + r) / 3;
//
//				if (p <= 5)
//					m_Particle[i].life = false;
//
//				if (m_Particle[i].life == true)
//				{
//					Vector2f pt = m_Particle[i].Pos - Goal;
//					float lpt = pt.Length();
//					if (lpt <= 10)
//						IsGoal = true;
//					if (lenMax >= lpt)
//					{
//						lenMax = lpt;
//						id = i;
//					}
//					circle(tmap, Point(m_Particle[i].Pos.x, m_Particle[i].Pos.y), 3, Scalar(0, 255, 0), -1);
//					/*Response += Vector2f(m_Particle[i].Pos.x, m_Particle[i].Pos.y);
//					AliveCount++;*/
//				}
//				
//			}
//			else
//			{
//				m_Particle[i].life = false;
//			}
//
//			if (m_Particle[i].life == false)
//			{
//				m_Particle[i].Pos = m_Particle[id].Pos;
//				float r = (rand() % 360) / 180.0f * 3.14;
//				m_Particle[i].Dir = Vector2f(sin(r), cos(r));
//				m_Particle[i].Speed = rand() % 5 + 1;
//				m_Particle[i].life = true;
//			}
//		}
//		/*if (AliveCount > 0.0f)
//		{
//			Response /= AliveCount;
//		}
//		for(int i = 0; i < m_Particle.size(); i++)
//		{
//			if(m_Particle[i].life == false)
//			{
//				m_Particle[i].Pos = Response;
//			}
//		}
//		Response = Vector2f(0,0);
//		AliveCount = 0.0f;*/
//
//		circle(tmap, Point(Goal.x, Goal.y), 10, Scalar(255, 0, 0), -1);
//
//		char str[255];
//		sprintf(str, "Time: %d", time);
//		string tex = str;
//		putText(tmap, tex, Point(400, .30), 1, 2, Scalar(255, 0, 255));
//
//		imshow("Particle", tmap);
//		cvWaitKey(10);
//		/*if (IsGoal == true)
//			cvWaitKey(100000000);*/
//		time++;
//	}
//
//	m_Particle.clear();
//	return 0; 
//}

struct PARTICLE
{
	Vector2f Pos;
	Vector2f Dir;
	float Speed;
	float MaxLen;
};

vector<PARTICLE> m_Particle;
Vector2f Goal = Vector2f(300, 300);

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		Goal = Vector2f(x, y);
		for (int i = 0;i < m_Particle.size();i++)
		{
			Vector2f p = m_Particle[i].Pos - Goal;
			m_Particle[i].MaxLen = p.Length();
 		}
	}
}

int main()
{
	srand(time(NULL));
	m_Particle.clear();

	for (int i = 0; i < 50;i++)
	{
		PARTICLE a;
		a.Pos = Vector2f(300, 300);
		float r = (rand() % 360) / 180.0f * 3.14;
		a.Dir = Vector2f(sin(r), cos(r));
		a.Speed = rand() % 5 + 1;
		a.MaxLen = 10000;
		m_Particle.push_back(a);
	}

	Mat map = Mat(600, 600, CV_8UC3, Scalar(200, 200, 200));
	namedWindow("Particle", 1);
	setMouseCallback("Particle", CallBackFunc, NULL);
	int time = 0;

	while (1)
	{
		Mat tmap = map.clone();

		for (int i = 0; i < m_Particle.size();i++)
		{
			if (time % 10 == 0)
			{
				float r = (rand() % 360) / 180.0f * 3.14;
				m_Particle[i].Dir = Vector2f(sin(r), cos(r));
				m_Particle[i].Speed = rand() % 5 + 1;
			}

			Vector2f p = m_Particle[i].Pos - Goal;
			float len = p.Length() / m_Particle[i].MaxLen;
			Vector2f pt = Goal - m_Particle[i].Pos;
			pt.Normalize();
			Vector2f dir = m_Particle[i].Dir * (1.0f - len) + pt * len;
			dir.Normalize();
			m_Particle[i].Dir += dir * m_Particle[i].Speed;
			circle(tmap, Point(m_Particle[i].Pos.x, m_Particle[i].Pos.y), 3, Scalar(0, 255, 0), -1);
 
		}


		circle(tmap, Point(Goal.x, Goal.y), 5, Scalar(255, 0, 0), -1);
		char str[255];
		sprintf(str, "TIME : %d", time);
		string tex = str;
		putText(tmap, tex, Point(400, 30), 1, 2, Scalar(255, 0, 255));
		imshow("Particle", tmap);
		cvWaitKey(10);

		time++;
	}

	return 0;
}