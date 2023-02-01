#include <stdio.h>
#include "OpenCV\InitOpenCV.h"
#include "vector.h"
#include <vector>
#include "LUA/InitLua.h"

using namespace std;

#define PARTICLE_NUM 100

//step1
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

//step2

//struct PARTICLE
//{
//	Vector2f Pos;
//	Vector2f Dir;
//	float Speed;
//	bool life;
//};
//
//PARTICLE g_Particle[PARTICLE_NUM];
//Vector2f g_GoalPos = Vector2f(0,0);
//bool g_IsGoal = false;
//Mat g_map;
//
//extern "C" int _Printf(lua_State * L)
//{
//	const char* data = luaL_optstring(L, 1, 0);
//	printf("%s \n", data);
//
//	return 0;
//}
//
//extern "C" int _SetParticle(lua_State * L)
//{
//	int id = (int)luaL_optint(L, 1, 0);
//	float posx = (float)luaL_optnumber(L, 2, 0);
//	float posy = (float)luaL_optnumber(L, 3, 0);
//	float dirx = (float)luaL_optnumber(L, 4, 0);
//	float diry = (float)luaL_optnumber(L, 5, 0);
//	float speed = (float)luaL_optnumber(L, 6, 0);
//	bool life = (bool)luaL_optint(L, 7, 0);
//	g_Particle[id].Pos = Vector2f(posx, posy);
//	g_Particle[id].Dir = Vector2f(dirx, diry);
//	g_Particle[id].Speed = speed;
//	g_Particle[id].life = life;
//
//	return 0;
//}
//
//extern "C" int _GetParticle(lua_State * L)
//{
//	int id = (int)luaL_optint(L, 1, 0);
//	lua_pushnumber(L, g_Particle[id].Pos.x);
//	lua_pushnumber(L, g_Particle[id].Pos.y);
//	lua_pushnumber(L, g_Particle[id].Dir.x);
//	lua_pushnumber(L, g_Particle[id].Dir.y);
//	lua_pushnumber(L, g_Particle[id].Speed);
//	lua_pushnumber(L, g_Particle[id].life);
//
//	return 6;
//}
//
//extern "C" int _GetGoalPos(lua_State* L)
//{
//	lua_pushnumber(L, g_GoalPos.x);
//	lua_pushnumber(L, g_GoalPos.y);
//
//	return 2;
//}
//
//extern "C" int _SetGoal(lua_State * L)
//{
//	g_IsGoal = (bool)luaL_optint(L, 1, 0);
//
//	return 0;
//}
//
//extern "C" int _GetParicleSize(lua_State * L)
//{
//	lua_pushinteger(L, PARTICLE_NUM);
//	return 1;
//}
//
//extern "C" int _GetCollision(lua_State * L)
//{
//	int x = (int)luaL_optnumber(L, 1, 0);
//	int y = (int)luaL_optnumber(L, 2, 0);
//
//	int b = g_map.at<Vec3b>(y, x)[0];
//	int g = g_map.at<Vec3b>(y, x)[1];
//	int r = g_map.at<Vec3b>(y, x)[2];
//	int p = b + g + r;
//	if (p <= 10)
//		lua_pushinteger(L, 1);
//	else
//		lua_pushinteger(L, 0);
//
//	return 1;
//}
//
//extern "C" int _GetLength(lua_State * L)
//{
//	float x = (float)luaL_optnumber(L, 1, 0);
//	float y = (float)luaL_optnumber(L, 2, 0);
//	float t = sqrt((x * x) + (y * y));
//	lua_pushnumber(L, t);
//	return 1;
//}
//
//lua_State* InitLua(const char* Luafile)
//{
//	luaL_reg Luafunc[] =
//	{
//		{"printf", _Printf},
//		{"SetParticle", _SetParticle},
//		{"GetParticle", _GetParticle},
//		{"GetGoalPos", _GetGoalPos},
//		{"SetGoal", _SetGoal},
//		{"GetParticleSize", _GetParicleSize},
//		{"GetCollision", _GetCollision},
//		{"GetLength", _GetLength},	
//
//
//	};
//	int Luafunc_Size = sizeof(Luafunc) / sizeof(luaL_reg);
//
//	lua_State* L = lua_open();
//	luaL_openlibs(L);
//	printf("Lua Version : %s \n", LUA_VERSION);
//	for (int i = 0;i < Luafunc_Size;i++)
//	{
//		lua_register(L, Luafunc[i].name, Luafunc[i].func);
//	}
//
//	int state = luaL_loadfile(L, Luafile);
//	lua_pcall(L, 0, 0, 0);
//
//	return L;
//}
//void onCallInit(lua_State* L)
//{
//	for (int i = 0; i < PARTICLE_NUM; i++)
//	{
//		lua_getglobal(L, "onInit");
//		lua_pushinteger(L, i);
//		lua_call(L, 1, 0);
//	}
//}
//void onCallFrameMove(lua_State* L, int time)
//{
//	for (int i = 0; i < PARTICLE_NUM; i++)
//	{
//		lua_getglobal(L, "onFrameMove");
//		lua_pushinteger(L, i);
//		lua_pushinteger(L, time);
//		lua_call(L, 2, 0);
//	}
//}
//
//void GetGoalPos()
//{
//	g_GoalPos = Vector2f(0, 0);
//
//	float count = 0;
//
//	for (int x = 0; x < g_map.cols; x += 1)
//	{
//		for (int y = 0; y < g_map.rows; y += 1)
//		{
//			int b = g_map.at<Vec3b>(y, x)[0];
//			int g = g_map.at<Vec3b>(y, x)[1];
//			int r = g_map.at<Vec3b>(y, x)[2];
//			if (r > g && r > b)
//			{
//				g_GoalPos += Vector2f(x, y);
//				count++;
//			}
//		}
//	}
//	g_GoalPos /= count;
//}
//
//void CallBackFunc(int event, int x, int y, int flags, void* userdata)
//{
//	if (event == EVENT_LBUTTONDOWN)
//	{
//		g_GoalPos = Vector2f(x, y);
//	}
//}
//
//int main()
//{
//	g_GoalPos = Vector2f(0, 0);
//	g_IsGoal = false;
//	g_map = imread("map.png");
//	namedWindow("Particle", 1);
//	setMouseCallback("Particle", CallBackFunc, NULL);
//
//	lua_State* Lua_Handle = InitLua("Particle.lua");
//	onCallInit(Lua_Handle);
//	GetGoalPos();
//
//	int time = 0;
//	while (1)
//	{
//		Mat tmap = g_map.clone();
//
//		onCallFrameMove(Lua_Handle, time);
//
//		for (int i = 0; i < PARTICLE_NUM; i++)
//		{
//			if (g_Particle[i].life == true)
//			{
//				circle(tmap, Point(g_Particle[i].Pos.x, g_Particle[i].Pos.y), 3, Scalar(0, 255, 0), -1);
//			}
//
//		}
//
//		circle(tmap, Point(g_GoalPos.x, g_GoalPos.y), 10, Scalar(255, 0, 0), -1);
//		char str[255];
//		sprintf(str, "TIME : %d", time);
//		string tex = str;
//		putText(tmap, tex, Point(400, 30), 1, 2, Scalar(255, 0, 255));
//
//		imshow("Particle", tmap);
//		cvWaitKey(10);
//		/*if (g_IsGoal == true)
//			cvWaitKey(100000000);*/
//		time++;
//	}
//
//	_sleep(100000);
//	lua_close(Lua_Handle);
//	return 0;
//}


struct PARTICLE
{
	Vector2f Pos;
	Vector2f Dir;
	float Speed;
	bool MaxLen;
};

PARTICLE g_Particle[PARTICLE_NUM];
Vector2f g_GoalPos = Vector2f(0,0);
bool g_IsGoal = false;
Mat g_map;

extern "C" int _Printf(lua_State * L)
{
	const char* data = luaL_optstring(L, 1, 0);
	printf("%s \n", data);

	return 0;
}

extern "C" int _SetParticle(lua_State * L)
{
	int id = (int)luaL_optint(L, 1, 0);
	float posx = (float)luaL_optnumber(L, 2, 0);
	float posy = (float)luaL_optnumber(L, 3, 0);
	float dirx = (float)luaL_optnumber(L, 4, 0);
	float diry = (float)luaL_optnumber(L, 5, 0);
	float speed = (float)luaL_optnumber(L, 6, 0);
	float maxLen = (float)luaL_optnumber(L, 7, 0);
	g_Particle[id].Pos = Vector2f(posx, posy);
	g_Particle[id].Dir = Vector2f(dirx, diry);
	g_Particle[id].Speed = speed;
	g_Particle[id].MaxLen = maxLen;

	return 0;
}

extern "C" int _GetParticle(lua_State * L)
{
	int id = (int)luaL_optint(L, 1, 0);
	lua_pushnumber(L, g_Particle[id].Pos.x);
	lua_pushnumber(L, g_Particle[id].Pos.y);
	lua_pushnumber(L, g_Particle[id].Dir.x);
	lua_pushnumber(L, g_Particle[id].Dir.y);
	lua_pushnumber(L, g_Particle[id].Speed);
	lua_pushnumber(L, g_Particle[id].MaxLen);

	return 6;
}

extern "C" int _GetGoalPos(lua_State* L)
{
	lua_pushnumber(L, g_GoalPos.x);
	lua_pushnumber(L, g_GoalPos.y);

	return 2;
}

extern "C" int _GetLength(lua_State * L)
{
	float x = (float)luaL_optnumber(L, 1, 0);
	float y = (float)luaL_optnumber(L, 2, 0);
	float t = sqrt((x * x) + (y * y));
	lua_pushnumber(L, t);
	return 1;
}


extern "C" int _Normalize(lua_State * L)
{
	float x = (float)luaL_optnumber(L, 1, 0);
	float y = (float)luaL_optnumber(L, 2, 0);
	float k = sqrt((x * x) + (y * y));

	if (k == 0.0)
	{
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
	}
	else
	{
		x = x / k;
		y = y / k;
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);		
	}
	return 2;
}

lua_State* InitLua(const char* Luafile)
{
	luaL_reg Luafunc[] =
	{
		{"printf", _Printf},
		{"SetParticle", _SetParticle},
		{"GetParticle", _GetParticle},
		{"GetGoalPos", _GetGoalPos},
		{"Normalize", _Normalize},
		{"GetLength", _GetLength},
	};

	int Luafunc_Size = sizeof(Luafunc) / sizeof(luaL_reg);

	lua_State* L = lua_open();
	luaL_openlibs(L);
	printf("Lua Version : %s \n", LUA_VERSION);
	for (int i = 0;i < Luafunc_Size;i++)
	{
		lua_register(L, Luafunc[i].name, Luafunc[i].func);
	}

	int state = luaL_loadfile(L, Luafile);
	lua_pcall(L, 0, 0, 0);

	return L;
}

void onCallInit(lua_State* L)
{
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		lua_getglobal(L, "onInit");
		lua_pushinteger(L, i);
		lua_call(L, 1, 0);
	}
}
void onCallFrameMove(lua_State* L, int time)
{
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		lua_getglobal(L, "onFrameMove");
		lua_pushinteger(L, i);
		lua_pushinteger(L, time);
		lua_call(L, 2, 0);
	}
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		g_GoalPos = Vector2f(x, y);

		for (int i = 0; i < PARTICLE_NUM; i++)
		{
			Vector2f p = g_Particle[i].Pos - g_GoalPos;
			g_Particle[i].MaxLen = p.Length();
		}
	}
}

int main()
{
	g_GoalPos = Vector2f(0, 0);
	g_IsGoal = false;
	g_map = Mat(600, 600, CV_8UC3, Scalar(150, 150, 150));
	namedWindow("Particle", 1);
	setMouseCallback("Particle", CallBackFunc, NULL);
	
	lua_State* Lua_Handle = InitLua("Particle.lua");
	onCallInit(Lua_Handle);
	
	int time = 0;
	while (1)
	{
		Mat tmap = g_map.clone();
	
		onCallFrameMove(Lua_Handle, time);
	
		for (int i = 0; i < PARTICLE_NUM; i++)
		{
			circle(tmap, Point(g_Particle[i].Pos.x, g_Particle[i].Pos.y), 3, Scalar(0, 255, 0), -1);	
		}
	
		circle(tmap, Point(g_GoalPos.x, g_GoalPos.y), 10, Scalar(255, 0, 0), -1);
		char str[255];
		sprintf(str, "TIME : %d", time);
		string tex = str;
		putText(tmap, tex, Point(400, 30), 1, 2, Scalar(255, 0, 255));
	
		imshow("Particle", tmap);
		cvWaitKey(10);
		
		time++;
	}
	
	
	lua_close(Lua_Handle);
	

	return 0;
}