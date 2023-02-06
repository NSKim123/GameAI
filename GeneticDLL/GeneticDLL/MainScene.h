#pragma warning(disable:4996)
#include "vec.h"
#include <vector>
using namespace std;

#ifndef MAINSCENE_H__
#define MAINSCENE_H__

struct STATE
{
	vec3 Pos;
	vec3 Dir;
	int State;
};
struct GENE
{
	int g[5];
	float re;
};

class CMainScene
{
private:
	vector<GENE> m_Gene;
	int m_DoGeneCount;
	int m_beforeDoGeneCount;
	vec3 m_HeroPos;
	vec3 m_EnemyPos;
	vec3 m_EnemyDir;
	vec3 m_EnemyGoalPos;
	int m_nState;

	vec3 m_HeroIndex;
	int m_nHeroHP;
	int m_nEnemyHP;

	int m_nAttackTime;
	int m_MapData[5][5];

	//GA
	int m_nMaxgeneration;

private:
	vec3 GetVec3Pos(int x, int y);
	vec3 GetIndexPos(vec3 pos);
	//GA
	void OnInitGene();
	void OnMatingGene();
	void OnReleaseGene();
	bool GetCheckGene(int i);
	void onCreateGene();

public:
	CMainScene();
	~CMainScene();

	void init();
	void Release();
	void FrameMove();
	void SetHeroMove(vec3 pos, int hp);
	void SetEnemyInfo(int hp);
	STATE GetState();
	int GetMap(int y, int x);


};

#endif