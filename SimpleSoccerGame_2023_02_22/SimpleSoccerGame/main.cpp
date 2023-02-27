#include <stdio.h>
#include <Windows.h>
#include "OpenCV\InitOpenCV.h"
#include "global.h"
#include "Lua.h"
#include <vector>

#define HOME false
#define AWAY true


using namespace std;


void DrawMap(Mat& tmap);
void DrawAgent(Mat& tmap);
void DrawText(Mat& tmap, int time);

void onInit(lua_State* L);
void FrameMove(lua_State* HL, lua_State* AL);

void AllAgentStateInit();

int main()
{
	char Home_Str[255];
	char Away_Str[255];
	memset(Home_Str, 0, 255);
	memset(Away_Str, 0, 255);
	FILE* fp = fopen("Setting.ini", "r");
	fgets(Home_Str, 255, fp);
	fgets(Away_Str, 255, fp);
	for (int i = 0; i < 255; i++)
	{
		if (Home_Str[i] == 10)
			Home_Str[i] = 0;
		if (Away_Str[i] == 10)
			Away_Str[i] = 0;
	}
	fclose(fp);
	printf("HOME: %s   AWAY: %s \n", Home_Str, Away_Str);

	srand(time(NULL));
	lua_State* Home_Lua_Handle = InitLua(Home_Str, 0);
	lua_State* Away_Lua_Handle = InitLua(Away_Str, 1);

	onInit(Home_Lua_Handle);
	onInit(Away_Lua_Handle);

	int	 time = GetTickCount();
	while (1)
	{
		FrameMove(Home_Lua_Handle, Away_Lua_Handle);
		Mat tmap = g_map.clone();
		DrawMap(tmap);
		DrawAgent(tmap);
		DrawText(tmap, time);
		imshow("SoccerGame", tmap);
		int key = cvWaitKey(1);
		if (key == 32)
		{
			lua_close(Home_Lua_Handle);
			lua_close(Away_Lua_Handle);

			memset(Home_Str, 0, 255);
			memset(Away_Str, 0, 255);

			fp = fopen("Setting.ini", "r");
			fgets(Home_Str, 255, fp);
			fgets(Away_Str, 255, fp);
			fclose(fp);
			for (int i = 0; i < 255; i++)
			{
				if (Home_Str[i] == 10)
					Home_Str[i] = 0;
				if (Away_Str[i] == 10)
					Away_Str[i] = 0;
			}
			printf("HOME: %s   AWAY: %s \n", Home_Str, Away_Str);
			Home_Lua_Handle = InitLua(Home_Str, 0);
			Away_Lua_Handle = InitLua(Away_Str, 1);
			onInit(Home_Lua_Handle);
			onInit(Away_Lua_Handle);
			time = GetTickCount();
		}
	}
	lua_close(Home_Lua_Handle);
	lua_close(Away_Lua_Handle);
	return 0;
}

void onInit(lua_State* L)
{
	g_GameTime = 0;
	float TotalSpeed = 2;
	g_GoalTime = 0;
	g_map = imread("map.png");
	g_CenterPos = Vector2f(g_map.cols / 2.0f, g_map.rows / 2.0f);
	g_Ball.InitPos = g_CenterPos;
	g_Ball.Pos = g_CenterPos;
	g_Ball.State = -1;
	g_Ball.TargetId = -1;

	g_WhoAttack = false;
	g_GameState = -1;
	g_HomeScore = 0;
	g_AwayScore = 0;
	g_isWhoGoal = HOME;

	g_HomePlayer[0].InitPos = Vector2f(30, g_CenterPos.y);
	g_HomePlayer[1].InitPos = Vector2f(200, g_CenterPos.y - 150);
	g_HomePlayer[2].InitPos = Vector2f(200, g_CenterPos.y + 150);
	g_HomePlayer[3].InitPos = Vector2f(400, g_CenterPos.y - 150);
	g_HomePlayer[4].InitPos = Vector2f(400, g_CenterPos.y + 150);

	g_AwayPlayer[0].InitPos = Vector2f(g_map.cols - 30, g_CenterPos.y);
	g_AwayPlayer[1].InitPos = Vector2f(g_map.cols - 200, g_CenterPos.y - 150);
	g_AwayPlayer[2].InitPos = Vector2f(g_map.cols - 200, g_CenterPos.y + 150);
	g_AwayPlayer[3].InitPos = Vector2f(g_map.cols - 400, g_CenterPos.y - 150);
	g_AwayPlayer[4].InitPos = Vector2f(g_map.cols - 400, g_CenterPos.y + 150);

	for (int i = 0; i < 5; i++)
	{
		g_HomePlayer[i].ID = i + 10;
		g_AwayPlayer[i].ID = i + 20;
		memset(g_HomePlayer[i].name, 0, 50);
		memset(g_AwayPlayer[i].name, 0, 50);
		if (i == 0)
		{
			sprintf(g_HomePlayer[i].name, "G1");
			sprintf(g_AwayPlayer[i].name, "G1");
		}
		if (i == 1)
		{
			sprintf(g_HomePlayer[i].name, "D1");
			sprintf(g_AwayPlayer[i].name, "D1");
		}
		if (i == 2)
		{
			sprintf(g_HomePlayer[i].name, "D2");
			sprintf(g_AwayPlayer[i].name, "D2");
		}
		if (i == 3)
		{
			sprintf(g_HomePlayer[i].name, "A1");
			sprintf(g_AwayPlayer[i].name, "A1");
		}
		if (i == 4)
		{
			sprintf(g_HomePlayer[i].name, "A2");
			sprintf(g_AwayPlayer[i].name, "A2");
		}


		g_HomePlayer[i].State = -1;
		g_HomePlayer[i].Speed = TotalSpeed;
		g_AwayPlayer[i].State = -1;
		g_AwayPlayer[i].Speed = TotalSpeed;
		g_AwayPlayer[i].Pos = g_CenterPos;
		g_HomePlayer[i].Pos = g_CenterPos;
	}


	lua_getglobal(L, "onInit");
	lua_call(L, 0, 0);

}

void InitGame()
{
	if (g_GameState == -1)
	{
		g_Ball.State = -1;
		g_Ball.Power = Vector2f(0, 0);
		g_Ball.Pos = g_CenterPos;
		int ReadyNum = 0;
		for (int i = 0; i < 5; i++)
		{
	
			if (g_HomePlayer[i].State != -1)
				ReadyNum++;
			if (g_AwayPlayer[i].State != -1)
				ReadyNum++;

			g_HomePlayer[i].GetBall = false;
			g_AwayPlayer[i].GetBall = false;

			if (g_HomePlayer[i].State == -1)
			{

				Vector2f p = g_HomePlayer[i].InitPos - g_HomePlayer[i].Pos;
				if (p.Length() > 5.0)
				{
					g_HomePlayer[i].Dir = p.Normalize();
					g_HomePlayer[i].Pos += g_HomePlayer[i].Dir * 4;
				}
				else
				{
					g_HomePlayer[i].Pos = g_HomePlayer[i].InitPos;
					g_HomePlayer[i].State = 0;
				}
			}
			if (g_AwayPlayer[i].State == -1)
			{
				Vector2f p = g_AwayPlayer[i].InitPos - g_AwayPlayer[i].Pos;
				if (p.Length() > 5.0)
				{
					g_AwayPlayer[i].Dir = p.Normalize();
					g_AwayPlayer[i].Pos += g_AwayPlayer[i].Dir * 4;
				}
				else
				{
					g_AwayPlayer[i].Pos = g_AwayPlayer[i].InitPos;
					g_AwayPlayer[i].State = 0;
				}
			}
		}
		if (ReadyNum == 10)
		{
			for (int i = 0; i < 5; i++)
			{
				g_AwayPlayer[i].Speed = 0;
				g_HomePlayer[i].Speed = 0;
			}
			if (g_isWhoGoal == false)
			{
				g_GameState = 1;
			}
			else
			{
				g_GameState = 2;
			}
		}

	}
}

void Home_Kickoff()
{
	if (g_GameState == 1)
	{
		Vector2f p = g_Ball.Pos - g_HomePlayer[3].Pos;
		if (p.Length() < 3.0)
		{
			g_Ball.State = g_HomePlayer[3].ID;
			g_GameState = 0;
		}
		g_HomePlayer[3].Dir = p.Normalize();
		g_HomePlayer[3].Pos += g_HomePlayer[3].Dir * 2;
	}
}
void Away_Kickoff()
{
	if (g_GameState == 2)
	{
		Vector2f p = g_Ball.Pos - g_AwayPlayer[3].Pos;
		if (p.Length() < 3.0)
		{
			g_Ball.State = g_AwayPlayer[3].ID;
			g_GameState = 0;
		}
		g_AwayPlayer[3].Dir = p.Normalize();
		g_AwayPlayer[3].Pos += g_AwayPlayer[3].Dir * 2;
	}
}

void BallMove()
{
	InitGetBallState();
	if (g_Ball.TargetId != -1)
	{
		int id = g_Ball.TargetId / 10;
		int i = g_Ball.TargetId % 10;
		if (id == 1)
		{
			g_Ball.Power = (g_HomePlayer[i].Pos+Vector2f(rand()%10, rand() % 10)- g_Ball.Pos)*0.05f;
			if (g_Ball.Power.Length() <= 1.5)
				g_Ball.Power= g_Ball.Power.Normalize()*10.0;
		}
		if (id == 2)
		{
			g_Ball.Power = (g_AwayPlayer[i].Pos+ Vector2f(rand() % 10, rand() % 10) - g_Ball.Pos) * 0.05f;
			if (g_Ball.Power.Length() <= 1.5)
				g_Ball.Power =g_Ball.Power.Normalize() * 10.0;
		}
	}
	g_Ball.Pos += g_Ball.Power;

	if (g_Ball.State != -1)
	{
		int id = g_Ball.State / 10;
		int i = g_Ball.State % 10;

		if (id == 1)
		{
			g_Ball.TargetId = -1;
			g_Ball.Pos = g_HomePlayer[i].Pos;
		}
		if (id == 2)
		{
			g_Ball.Pos = g_AwayPlayer[i].Pos;
			g_Ball.TargetId = -1;
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			Vector2f p = g_Ball.Pos - g_HomePlayer[i].Pos;
			if (i == 0)
				p *= 0.5;

			if (p.Length() < 3.0)
			{
				g_Ball.State = g_HomePlayer[i].ID;
				break;
			}
			p = g_Ball.Pos - g_AwayPlayer[i].Pos;
			if (i == 0)
				p *= 0.5;

			if (p.Length() < 3.0)
			{
				g_Ball.State = g_AwayPlayer[i].ID;
				break;
			}

		}
	}


	if (g_Ball.Power.Length() <= 0.01)
		g_Ball.Power = Vector2f(0, 0);
	else
		g_Ball.Power *= 0.98f;

	if (g_GameState == 0)
	{
		if (g_Ball.Pos.x <= 10 && g_Ball.Pos.y >= (g_CenterPos.y - 100) && g_Ball.Pos.y <= (g_CenterPos.y + 100))
		{
			g_AwayScore += 1;
			g_isWhoGoal = HOME;
			g_GameState = 4;
			g_GoalTime = GetTickCount();
		}
		if (g_Ball.Pos.x >= (g_map.cols - 10) && g_Ball.Pos.y >= (g_CenterPos.y - 100) && g_Ball.Pos.y <= (g_CenterPos.y + 100))
		{
			g_HomeScore += 1;
			g_isWhoGoal = AWAY;
			g_GameState = 3;
			g_GoalTime = GetTickCount();
		}
	}

	if (g_Ball.Pos.x <= 10)
		AllAgentStateInit();
	if (g_Ball.Pos.x >= (g_map.cols - 10))
		AllAgentStateInit();
	if (g_Ball.Pos.y <= 10)
		AllAgentStateInit();
	if (g_Ball.Pos.y >= (g_map.rows - 10))
		AllAgentStateInit();

}

void GoalkeeperMove()
{
	if (g_GameState != -1)
	{

		g_HomePlayer[0].Pos.x = 30;
		g_AwayPlayer[0].Pos.x = g_map.cols - 30;

		if (g_HomePlayer[0].Pos.y <= (g_CenterPos.y - 100))
			g_HomePlayer[0].Pos.y = (g_CenterPos.y - 100);
		if (g_HomePlayer[0].Pos.y >= (g_CenterPos.y + 100))
			g_HomePlayer[0].Pos.y = (g_CenterPos.y + 100);

		if (g_AwayPlayer[0].Pos.y <= (g_CenterPos.y - 100))
			g_AwayPlayer[0].Pos.y = (g_CenterPos.y - 100);
		if (g_AwayPlayer[0].Pos.y >= (g_CenterPos.y + 100))
			g_AwayPlayer[0].Pos.y = (g_CenterPos.y + 100);

	}
}

void AllAgentStateInit()
{
	g_GameState = -1;
	for (int i = 0; i < 5; i++)
	{
		g_HomePlayer[i].State = -1;
		g_AwayPlayer[i].State = -1;
	}
}

void FrameMove(lua_State* HL, lua_State* AL)
{
	if (g_GameState == 0)
	{
		lua_getglobal(HL, "onFrameMove");
		lua_call(HL, 0, 0);
		lua_getglobal(AL, "onFrameMove");
		lua_call(AL, 0, 0);

		float size = 10.0f;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (i != j)
				{
					float len = (g_HomePlayer[i].Pos - g_HomePlayer[j].Pos).Length();
					if (len <= size)
					{

						float tlen = size - len;
						Vector2f dir = g_HomePlayer[i].Pos - g_HomePlayer[j].Pos;
						if (len <= 0)
						{
							dir = Vector2f(0, 1);
							tlen = size;
						}
						dir.Normalize();
						g_HomePlayer[j].Pos -= (dir * tlen);
					}
				}
			}
			for (int j = 0; j < 5; j++)
			{
				if (i != j)
				{
					float len = (g_AwayPlayer[i].Pos - g_AwayPlayer[j].Pos).Length();
					if (len <= size)
					{

						float tlen = size - len;
						Vector2f dir = g_AwayPlayer[i].Pos - g_AwayPlayer[j].Pos;
						if (len <= 0)
						{
							dir = Vector2f(0, 1);
							tlen = size;
						}
						dir.Normalize();
						g_AwayPlayer[j].Pos -= (dir * tlen);
					}
				}
			}
		}

		for (int i = 0; i < 5; i++)
		{
			

			g_HomePlayer[i].Pos += g_HomePlayer[i].Dir * g_HomePlayer[i].Speed;
			if (g_HomePlayer[i].Pos.x <= 30)
				g_HomePlayer[i].Pos.x = 30;
			if (g_HomePlayer[i].Pos.x >= (g_map.cols - 30))
				g_HomePlayer[i].Pos.x = (g_map.cols - 30);
			if (g_HomePlayer[i].Pos.y <= 40)
				g_HomePlayer[i].Pos.y = 40;
			if (g_HomePlayer[i].Pos.y >= (g_map.rows - 40))
				g_HomePlayer[i].Pos.y = (g_map.rows - 40);

			g_AwayPlayer[i].Pos += g_AwayPlayer[i].Dir * g_AwayPlayer[i].Speed;
			if (g_AwayPlayer[i].Pos.x <= 30)
				g_AwayPlayer[i].Pos.x = 30;
			if (g_AwayPlayer[i].Pos.x >= (g_map.cols - 30))
				g_AwayPlayer[i].Pos.x = (g_map.cols - 30);
			if (g_AwayPlayer[i].Pos.y <= 40)
				g_AwayPlayer[i].Pos.y = 40;
			if (g_AwayPlayer[i].Pos.y >= (g_map.rows - 40))
				g_AwayPlayer[i].Pos.y = (g_map.rows - 40);

			g_HomePlayer[i].Speed *= 0.5;
			g_AwayPlayer[i].Speed *= 0.5;
			if (g_HomePlayer[i].Speed <= 0.1)
				g_HomePlayer[i].Speed = 0;
			if (g_AwayPlayer[i].Speed <= 0.1)
				g_AwayPlayer[i].Speed = 0;
		}
	}



	InitGame();
	Home_Kickoff();
	Away_Kickoff();
	BallMove();
	GoalkeeperMove();
	

	if (g_GameState == 3 || g_GameState == 4)
	{
		int gtime = (GetTickCount() - g_GoalTime) / 1000;
		if (gtime >= 1)
			AllAgentStateInit();
	}

	
}

void DrawMap(Mat& tmap)
{
	rectangle(tmap, Point(5, g_CenterPos.y - 100), Point(25, g_CenterPos.y + 100), Scalar(200, 200, 200), -1);
	rectangle(tmap, Point(g_map.cols - 5, g_CenterPos.y - 100), Point(g_map.cols - 25, g_CenterPos.y + 100), Scalar(200, 200, 200), -1);
	circle(tmap, Point(30, 40), 5, Scalar(0, 255, 255), -1);
	circle(tmap, Point(30, g_map.rows - 40), 5, Scalar(0, 255, 255), -1);
	circle(tmap, Point(g_map.cols - 30, g_map.rows - 40), 5, Scalar(0, 255, 255), -1);
	circle(tmap, Point(g_map.cols - 30, 40), 5, Scalar(0, 255, 255), -1);
	circle(tmap, Point(g_CenterPos.x, g_CenterPos.y), 13, Scalar(255, 255, 255), -1);
}

void DrawAgent(Mat& tmap)
{
	for (int i = 0; i < 5; i++)
	{
		circle(tmap, Point(g_HomePlayer[i].Pos.x, g_HomePlayer[i].Pos.y), 15, Scalar(255, 0, 0), -1);
		putText(tmap, g_HomePlayer[i].name, Point(g_HomePlayer[i].Pos.x - 12, g_HomePlayer[i].Pos.y + 6), 1.2, 1.2, Scalar(255, 255, 255));

		circle(tmap, Point(g_AwayPlayer[i].Pos.x, g_AwayPlayer[i].Pos.y), 15, Scalar(0, 0, 255), -1);
		putText(tmap, g_AwayPlayer[i].name, Point(g_AwayPlayer[i].Pos.x - 12, g_AwayPlayer[i].Pos.y + 6), 1.2, 1.2, Scalar(255, 255, 255));
	}
	circle(tmap, Point(g_Ball.Pos.x, g_Ball.Pos.y), 8, Scalar(255, 255, 0), -1);
}

void DrawText(Mat& tmap, int time)
{
	rectangle(tmap, Point(0, 0), Point(g_map.cols, 25), Scalar(255, 255, 255), -1);
	char str[255];
	int ntime = (GetTickCount() - time) / 1000;
	g_GameTime = ntime;
	memset(str, 0, 255);
	sprintf(str, "TIME: %d:%d", ntime / 60, ntime % 60);
	string tex = str;
	putText(tmap, tex, Point(10, 20), 1.2, 1.5, Scalar(0, 0, 0));

	memset(str, 0, 255);

	if (g_GameState == 1)
	{
		sprintf(str, "Home Kick-off");
		tex = str;
		putText(tmap, tex, Point(800, 20), 1.2, 1.5, Scalar(255, 0, 0));
	}
	else if (g_GameState == 2)
	{
		sprintf(str, "Away Kick-off");
		tex = str;
		putText(tmap, tex, Point(800, 20), 1.2, 1.5, Scalar(0, 0, 255));
	}
	else if (g_GameState == 3)
	{
		sprintf(str, "Home Goal!!");
		tex = str;
		putText(tmap, tex, Point(800, 20), 1.2, 1.5, Scalar(255, 0, 0));
	}
	else if (g_GameState == 4)
	{
		sprintf(str, "Away Goal!!");
		tex = str;
		putText(tmap, tex, Point(800, 20), 1.2, 1.5, Scalar(0, 0, 255));
	}
	else if (g_GameState == -1)
	{
		sprintf(str, "Init Game");
		tex = str;
		putText(tmap, tex, Point(800, 20), 1.2, 1.5, Scalar(255, 0, 255));
	}
	else
	{
		sprintf(str, "Play Game");
		tex = str;
		if (g_WhoAttack == false)
		{
			putText(tmap, tex, Point(850, 20), 1.2, 1.5, Scalar(255, 0, 0));
		}
		else
		{
			putText(tmap, tex, Point(850, 20), 1.2, 1.5, Scalar(0, 0, 255));
		}
	}

	memset(str, 0, 255);
	sprintf(str, "Score: %d:%d", g_HomeScore, g_AwayScore);
	tex = str;
	putText(tmap, tex, Point(440, 20), 1.2, 1.5, Scalar(0, 0, 0));
}