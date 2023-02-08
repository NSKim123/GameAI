#include "pch.h"
#include "MainScene.h"
#include <time.h>
#include <direct.h>

CMainScene::CMainScene()
{

}
CMainScene::~CMainScene()
{

}

void CMainScene::init(int HeroCount, int EnemyCount)
{
	m_HeroList.clear();
	m_EnemyList.clear();

	for (int i = 0;i < HeroCount; i++)
	{
		TANK a;
		float x = (rand() % 100) - 50;
		a.Pos = vec3(x / 50.0 * 20.0f, 0, -19);
		a.Dir = vec3(0, 0, 1);
		a.State = 0;
		a.Speed = (rand() % 3 + 1) / 30.0f;
		a.HP = 100;
		a.TargetID = -1;
		m_HeroList.push_back(a);
	}
	for (int i = 0;i < EnemyCount; i++)
	{
		TANK b;
		float x = (rand() % 100) - 50;
		b.Pos = vec3(x / 50.0 * 20.0f, 0, 19);
		b.Dir = vec3(0, 0, 1);
		b.State = 0;
		b.Speed = (rand() % 3 + 1) / 30.0f;
		b.HP = 100;
		b.TargetID = -1;
		m_EnemyList.push_back(b);
	}
	printf("INIT!!\n");
}

void CMainScene::Release()
{
	m_HeroList.clear();
	m_EnemyList.clear();
	printf("Release()!!\n");
}

void CMainScene::SetHeroInfo(int id, int hp)
{
	m_HeroList[id].HP = hp;
}
void CMainScene::SetEnemyInfo(int id, int hp)
{
	m_EnemyList[id].HP = hp;
}

void CMainScene::FrameMove_Enemy()
{
	for (int i = 0; m_EnemyList.size();i++)
	{
		if (m_EnemyList[i].State != -1)
		{
			int id = -1;
			float fmin = 1000000;
			for (int j = 0;j < m_HeroList.size();j++)
			{
				if (m_HeroList[j].State != -1)
				{
					float len = length(m_EnemyList[i].Pos - m_HeroList[j].Pos);
					if (len <= fmin)
					{
						id = j;
						fmin = len;
					}
				}

			}
			if (id != -1)
			{
				vec3 dir = m_HeroList[id].Pos - m_EnemyList[i].Pos;
				m_EnemyList[i].Dir = normalize(dir);

				if (m_EnemyList[i].State == 1)
				{
					m_EnemyList[i].Pos += m_EnemyList[i].Dir * m_EnemyList[i].Speed;
				}
				if (m_EnemyList[i].State == 3)
				{
					m_EnemyList[i].Pos -= m_EnemyList[i].Dir * m_EnemyList[i].Speed;
				}
			}
			if (fmin <= 5.0f)
			{
				m_EnemyList[i].State = 2;
			}
			else
			{
				if (m_EnemyList[i].HP >= 40)
					m_EnemyList[i].State = 1;
				else
					m_EnemyList[i].State = 3;
			}

			if (m_EnemyList[i].State == 3)
			{
				m_EnemyList[i].HP++;
			}

			if (m_EnemyList[i].HP <= 0)
			{
				m_EnemyList[i].HP = 0;
				m_EnemyList[i].State = -1;
			}
		}
	}

	for (int i = 0; m_EnemyList.size();i++)
	{
		if (m_EnemyList[i].State != -1)
		{
			for (int j = 0;j < m_EnemyList.size(); j++)
			{
				if (m_EnemyList[j].State != -1 && i != j)
				{
					float len = length(m_EnemyList[i].Pos - m_EnemyList[j].Pos);
					if (len <= 1.2)
					{
						float tlen = 1.2f - len;
						vec3 dir = m_EnemyList[i].Pos - m_EnemyList[j].Pos;
						if (len <= 0)
						{
							dir = vec3(0, 0, 1);
							tlen = 1.2;
						}
						dir = normalize(dir);
						m_EnemyList[j].Pos -= (dir * tlen);
					}
				}
			}
		}
	}
}

void CMainScene::FrameMove_Hero()
{
	for (int i = 0; m_HeroList.size();i++)
	{
		if (m_HeroList[i].State != -1)
		{
			int id = -1;
			float fmin = 1000000;
			for (int j = 0;j < m_EnemyList.size();j++)
			{
				if (m_EnemyList[j].State != -1)
				{
					float len = length(m_HeroList[i].Pos - m_EnemyList[j].Pos);
					if (len <= fmin)
					{
						id = j;
						fmin = len;
					}
				}

			}
			if (id != -1)
			{
				vec3 dir = m_EnemyList[id].Pos - m_HeroList[i].Pos;
				m_HeroList[i].Dir = normalize(dir);

				if (m_HeroList[i].State == 1)
				{
					m_HeroList[i].Pos += m_HeroList[i].Dir * m_HeroList[i].Speed;
				}
				if (m_HeroList[i].State == 3)
				{
					m_HeroList[i].Pos -= m_HeroList[i].Dir * m_HeroList[i].Speed;
				}
			}
			if (fmin <= 5.0f)
			{
				m_HeroList[i].State = 2;
			}
			else
			{
				if (m_HeroList[i].HP >= 40)
					m_HeroList[i].State = 1;
				else
					m_HeroList[i].State = 3;
			}

			if (m_HeroList[i].State == 3)
			{
				m_HeroList[i].HP++;
			}

			if (m_HeroList[i].HP <= 0)
			{
				m_HeroList[i].HP = 0;
				m_HeroList[i].State = -1;
			}
		}
	}

	for (int i = 0; m_HeroList.size();i++)
	{
		if (m_HeroList[i].State != -1)
		{
			for (int j = 0;j < m_HeroList.size(); j++)
			{
				if (m_HeroList[j].State != -1 && i != j)
				{
					float len = length(m_HeroList[i].Pos - m_HeroList[j].Pos);
					if (len <= 1.2)
					{
						float tlen = 1.2f - len;
						vec3 dir = m_HeroList[i].Pos - m_HeroList[j].Pos;
						if (len <= 0)
						{
							dir = vec3(0, 0, 1);
							tlen = 1.2;
						}
						dir = normalize(dir);
						m_HeroList[j].Pos -= (dir * tlen);
					}
				}
			}
		}
	}
}

void CMainScene::FrameMove_Hero1()
{
	vec3 gpos = vec3(0, 0, 0);
	float gcount = 0;

	for (int i = 0;i < m_HeroList.size();i++)
	{
		if (m_HeroList[i].State != -1)
		{
			gpos += m_HeroList[i].Pos;
			gcount++;
		}
	}
	gpos /= gcount;

	for (int i = 0; m_HeroList.size();i++)
	{
		if (m_HeroList[i].State != -1)
		{
			int id = -1;
			float fmin = 1000000;
			for (int j = 0;j < m_EnemyList.size();j++)
			{
				if (m_EnemyList[j].State != -1)
				{
					float len = length(m_HeroList[i].Pos - m_EnemyList[j].Pos);
					if (len <= fmin)
					{
						id = j;
						fmin = len;
					}
				}

			}
			if (id != -1)
			{
				gpos.x = m_EnemyList[id].Pos.x;
				vec3 gdir = gpos - m_HeroList[i].Pos;
				vec3 ldir = m_EnemyList[id].Pos - m_HeroList[i].Pos;
				ldir = normalize(ldir);
				gdir = normalize(gdir);
				vec3 dir = (gdir * 0.7f) + (ldir * 0.3f);
				m_HeroList[i].Dir = normalize(dir);

				vec3 dir = m_EnemyList[id].Pos - m_HeroList[i].Pos;
				m_HeroList[i].Dir = normalize(dir);

				if (m_HeroList[i].State == 1)
				{
					m_HeroList[i].Pos += dir * m_HeroList[i].Speed;
				}
				if (m_HeroList[i].State == 3)
				{
					m_HeroList[i].Pos -= dir * m_HeroList[i].Speed;
				}
			}
			if (fmin <= 5.0f)
			{
				m_HeroList[i].State = 2;
			}
			else
			{
				if (m_HeroList[i].HP >= 40)
					m_HeroList[i].State = 1;
				else
					m_HeroList[i].State = 3;
			}

			if (m_HeroList[i].State == 3)
			{
				m_HeroList[i].HP++;
			}

			if (m_HeroList[i].HP <= 0)
			{
				m_HeroList[i].HP = 0;
				m_HeroList[i].State = -1;
			}
		}
	}

	for (int i = 0; m_HeroList.size();i++)
	{
		if (m_HeroList[i].State != -1)
		{
			for (int j = 0;j < m_HeroList.size(); j++)
			{
				if (m_HeroList[j].State != -1 && i != j)
				{
					float len = length(m_HeroList[i].Pos - m_HeroList[j].Pos);
					if (len <= 1.2)
					{
						float tlen = 1.2f - len;
						vec3 dir = m_HeroList[i].Pos - m_HeroList[j].Pos;
						if (len <= 0)
						{
							dir = vec3(0, 0, 1);
							tlen = 1.2;
						}
						dir = normalize(dir);
						m_HeroList[j].Pos -= (dir * tlen);
					}
				}
			}
		}
	}
}

int CMainScene::GetEnemyCount()
{
	int count = 0;
	for (int i = 0; i < m_EnemyList.size();i++)
	{
		if (m_EnemyList[i].State != -1)
		{
			count++;
		}
	}
	return count;
}
int CMainScene::GetHeroCount()
{
	int count = 0;
	for (int i = 0; i < m_HeroList.size();i++)
	{
		if (m_HeroList[i].State != -1)
		{
			count++;
		}
	}
	return count;
}

void CMainScene::FrameMove()
{
	FrameMove_Enemy();
	FrameMove_Hero1();
}

TANK CMainScene::GetHeroInfo(int id)
{
	return m_HeroList[id];
}
TANK CMainScene::GetEnemyInfo(int id)
{
	return m_EnemyList[id];
}