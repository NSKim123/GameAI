#ifndef GLOBAL_H__
#define GLOBAL_H__
#include<ctime>
#include "vector.h"
struct PLAYER
{
	char		name[50];
	int			ID;
	Vector2f	InitPos;
	Vector2f	Dir;
	Vector2f	Pos;
	float		Speed;
	int			State;
	bool		GetBall;

};

struct BALL
{
	Vector2f	InitPos;
	Vector2f	Pos;
	Vector2f	Power;
	int			State;
	int			TargetId;
};

Mat				g_map;
Vector2f		g_CenterPos;

BALL			g_Ball;
PLAYER			g_HomePlayer[5];
PLAYER			g_AwayPlayer[5];
int				g_GameState;
int				g_HomeScore;
int				g_AwayScore;
bool			g_isWhoGoal;
int				g_GoalTime;
bool			g_WhoAttack;
int				g_GameTime;

void InitGetBallState()
{
	for (int i = 0; i < 5; i++)
	{
		if (g_Ball.State == g_HomePlayer[i].ID)
		{
			g_HomePlayer[i].GetBall = true;
			g_WhoAttack = false;
		}
		else
			g_HomePlayer[i].GetBall = false;

		if (g_Ball.State == g_AwayPlayer[i].ID)
		{
			g_AwayPlayer[i].GetBall = true;
			g_WhoAttack = true;
		}
		else
			g_AwayPlayer[i].GetBall = false;
	}
}
#endif