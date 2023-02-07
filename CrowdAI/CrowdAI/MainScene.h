#include "vec.h"
#include <vector>
using namespace std;
#ifndef MAINSCENE_H__
#define MAINSCENE_H__

struct TANK
{
	vec3 Pos;
	vec3 Dir;
	float Speed;
	int State;
	int TargetID;
	int HP;
};

class CMainScene
{
private:
	vector<TANK> m_HeroList;
	vector<TANK> m_EnemyList;

private:
	void FrameMove_Enemy();
	void FrameMove_Hero();
	void FrameMove_Hero1();

public:
	CMainScene();
	~CMainScene();

	void init(int Herocount, int Enemycount);
	void Release();
	void FrameMove();
	void SetHeroInfo(int id, int hp);
	void SetEnemyInfo(int id, int hp);

	TANK GetHeroInfo(int id);
	TANK GetEnemyInfo(int id);
	int GetEnemyCount();
	int GetHeroCount();
};

#endif 

