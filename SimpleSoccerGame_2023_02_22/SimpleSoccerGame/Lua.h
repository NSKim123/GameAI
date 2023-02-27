#ifndef LUA_H__
#define LUA_H__
#include "LUA\InitLua.h"
#include "global.h"

extern "C" int _Printf(lua_State * L)
{
	const char* data = luaL_optstring(L, 1, 0);
	//printf("%s", data);
	return 0;
}

extern "C" int _Move(lua_State * L)
{
	int teamid = (int)luaL_optint(L, 1, 0);
	int id = (int)luaL_optint(L, 2, 0);
	float posx = (float)luaL_optnumber(L, 3, 0);
	float posy = (float)luaL_optnumber(L, 4, 0);
	if (teamid == 0)
	{
		g_HomePlayer[id].Dir = Vector2f(posx, posy);
		g_HomePlayer[id].Dir.Normalize();
		if(id!=0)
			g_HomePlayer[id].Speed = 2.0;
		else
			g_HomePlayer[id].Speed = 1.5;
	}
	else
	{
		g_AwayPlayer[id].Dir = Vector2f(posx, posy);
		g_AwayPlayer[id].Dir.Normalize();
		if (id != 0)
			g_AwayPlayer[id].Speed = 2.0;
		else
			g_AwayPlayer[id].Speed = 1.5;
	}
	
	return 0;
}

extern "C" int _GetHomeAgentPos(lua_State * L)
{
	int teamid = (int)luaL_optint(L, 1, 0);
	int id = (int)luaL_optint(L, 2, 0);
	if (teamid == 0)
	{
		lua_pushnumber(L, g_HomePlayer[id].Pos.x);
		lua_pushnumber(L, g_HomePlayer[id].Pos.y);
		lua_pushboolean(L, g_HomePlayer[id].GetBall);
	}
	else
	{
		lua_pushnumber(L, g_AwayPlayer[id].Pos.x);
		lua_pushnumber(L, g_AwayPlayer[id].Pos.y);
		lua_pushboolean(L, g_AwayPlayer[id].GetBall);
	}
	return 3;
}

extern "C" int _GetAwayAgentPos(lua_State * L)
{
	int teamid = (int)luaL_optint(L, 1, 0);
	int id = (int)luaL_optint(L, 2, 0);
	if (teamid == 0)
	{
		lua_pushnumber(L, g_AwayPlayer[id].Pos.x);
		lua_pushnumber(L, g_AwayPlayer[id].Pos.y);
		lua_pushboolean(L, g_AwayPlayer[id].GetBall);
	}
	else
	{
		lua_pushnumber(L, g_HomePlayer[id].Pos.x);
		lua_pushnumber(L, g_HomePlayer[id].Pos.y);
		lua_pushboolean(L, g_HomePlayer[id].GetBall);
	}
	return 3;
}

extern "C" int _GetBallPos(lua_State * L)
{
	lua_pushnumber(L, g_Ball.Pos.x);
	lua_pushnumber(L, g_Ball.Pos.y);
	lua_pushinteger(L, g_Ball.State);
	return 3;
}

extern "C" int _Pass(lua_State * L)
{
	int teamid = (int)luaL_optint(L, 1, 0);
	int id = (int)luaL_optint(L, 2, 0);
	int id1 = (int)luaL_optint(L, 3, 0);
	if (teamid == 0)
	{
		if(g_HomePlayer[id].GetBall==false)
			lua_pushboolean(L, false);
		else
		{
			Vector2f t= g_HomePlayer[id1].Pos-g_HomePlayer[id].Pos;
			g_Ball.Power = t*0.06f;
			g_Ball.TargetId = g_HomePlayer[id1].ID;
			g_Ball.State = -1;
			lua_pushboolean(L, true);
		}
	}
	else
	{
		if (g_AwayPlayer[id].GetBall == false)
			lua_pushboolean(L, false);
		else
		{
			Vector2f t = g_AwayPlayer[id1].Pos - g_AwayPlayer[id].Pos;
			g_Ball.Power = t * 0.06f;
			g_Ball.TargetId = g_AwayPlayer[id1].ID;
			g_Ball.State = -1;
			lua_pushboolean(L, true);
		}
	}
	return 1;
}

extern "C" int _Shoot(lua_State * L)
{
	int teamid = (int)luaL_optint(L, 1, 0);
	int id = (int)luaL_optint(L, 2, 0);
	float dy = (float)(rand() % 11-5.0f)*20;
	if (teamid == 0)
	{
		if (g_HomePlayer[id].GetBall == false)
			lua_pushboolean(L, false);
		else
		{
			
			Vector2f t =Vector2f(g_map.cols-25,g_CenterPos.y+ dy) - g_HomePlayer[id].Pos;
			t.Normalize();
			g_Ball.Power = t * 15.0f;
			g_Ball.TargetId = -1;
			g_Ball.State = -1;
			lua_pushboolean(L, true);
		}
	}
	else
	{
		if (g_AwayPlayer[id].GetBall == false)
			lua_pushboolean(L, false);
		else
		{
			Vector2f t = Vector2f(25, g_CenterPos.y + dy) - g_AwayPlayer[id].Pos;
			t.Normalize();
			g_Ball.Power = t * 15.0f;
			g_Ball.TargetId = -1;
			g_Ball.State = -1;
			lua_pushboolean(L, true);
		}
	}
	return 1;
}

extern "C" int _Tackle(lua_State * L)
{
	int teamid = (int)luaL_optint(L, 1, 0);
	int id = (int)luaL_optint(L, 2, 0);

	int tid = g_Ball.State / 10;
	int aid = g_Ball.State % 10;
	int temp = 10;
	if (g_Ball.State == -1)
	{
		lua_pushboolean(L, false);
		return 1;
	}
		
	if (teamid == 0)
	{
		if (g_HomePlayer[id].GetBall == true)
			lua_pushboolean(L, false);
		else
		{
			Vector2f t = g_Ball.Pos - g_HomePlayer[id].Pos;
			if(t.Length()>10)
				lua_pushboolean(L, false);
			else
			{
				int r = rand()%2;
				if (r == 1)
				{
					if (tid == 1)
					{
						g_HomePlayer[aid].Pos.x += -temp;
					}
					else
					{
						g_AwayPlayer[aid].Pos.x += temp;
					}
					g_Ball.State = g_HomePlayer[id].ID;
					InitGetBallState();
					lua_pushboolean(L, true);
				}
				else
					lua_pushboolean(L, false);
			}
		}
	}
	else
	{
		if (g_AwayPlayer[id].GetBall == true)
			lua_pushboolean(L, false);
		else
		{
			Vector2f t = g_Ball.Pos - g_AwayPlayer[id].Pos;
			if (t.Length() > 10)
				lua_pushboolean(L, false);
			else
			{
				
				int r = rand() % 2;
				if (r == 1)
				{
					if (tid == 1)
					{
						g_AwayPlayer[aid].Pos.x += temp;
					}
					else
					{
						g_HomePlayer[aid].Pos.x += -temp;
						
					}
					g_Ball.State = g_AwayPlayer[id].ID;
					InitGetBallState();
					lua_pushboolean(L, true);
				}
				else
					lua_pushboolean(L, false);
			}
		}
	}
	return 1;
}

extern "C" int _GetScore(lua_State * L)
{
	int teamid = (int)luaL_optint(L, 1, 0);

	if (teamid == 0)
	{
		lua_pushnumber(L, g_HomeScore);
		lua_pushnumber(L, g_AwayScore);
	}
	else
	{
		lua_pushnumber(L, g_AwayScore);
		lua_pushnumber(L, g_HomeScore);
	}
	return 2;
}

extern "C" int _WhoAttack(lua_State * L)
{
	int teamid = (int)luaL_optint(L, 1, 0);

	int whoball = g_WhoAttack;


	if (teamid == 0)
	{
		lua_pushnumber(L, whoball);
	}
	else
	{
		lua_pushnumber(L, (whoball+1)%2);
	}
	return 1;
}

extern "C" int _GetGameTime(lua_State * L)
{
	lua_pushnumber(L, g_GameTime);
	return 1;
}

lua_State* InitLua(const char* Luafile,int TeamId)
{
	luaL_reg Luafunc[] =
	{
		{"Tprintf",_Printf},
		{"TGetBallPos",_GetBallPos},
		{"Tmove",_Move},
		{"TGetHomeAgentPos",_GetHomeAgentPos},
		{"TGetAwayAgentPos",_GetAwayAgentPos},
		{"Tpass",_Pass},
		{"Tshoot",_Shoot},
		{"TTackle",_Tackle},
		{"TGetScore",_GetScore},
		{"TWhoAttack",_WhoAttack},
		{"TGetGameTime",_GetGameTime},
	};
	int Luafunc_Size = sizeof(Luafunc) / sizeof(luaL_reg);

	lua_State* L = lua_open();
	luaL_openlibs(L);
//	printf("Lua Version: %s \n", LUA_VERSION);
	for (int i = 0; i < Luafunc_Size; i++)
	{
		lua_register(L, Luafunc[i].name, Luafunc[i].func);
	}

	int state = luaL_loadfile(L, Luafile);
	lua_pcall(L, 0, 0, 0);

	lua_getglobal(L, "onSetTeamID");
	lua_pushinteger(L, TeamId);
	lua_call(L, 1, 0);

	return L;
}
#endif