#include "pch.h"
#include "MainScene.h"
#include <time.h>
#include <direct.h>
#include <algorithm>

bool comp(GENE a, GENE b)
{
	return (a.re > b.re);
}

CMainScene::CMainScene()
{

}
CMainScene::~CMainScene()
{

}

void CMainScene::init()
{
	m_Gene.clear();

	m_HeroPos = vec3(0, 0, 0);
	m_EnemyPos = vec3(0, 0, 0);
	m_nHeroHP = 100;
	m_nEnemyHP = 100;
	m_nState = - 1;
	for (int i = 0; i < 5;i++)
	{
		for (int j = 0; j < 5;j++)
		{
			m_MapData[i][j] = 0;
		}
	}
	FILE* fp = fopen("Map.txt", "r");
	char str[255];

	for (int i = 0;i < 5;i++)
	{
		fgets(str, 255, fp);
		sscanf(str, "%d %d %d %d %d", &m_MapData[i][0], &m_MapData[i][1], &m_MapData[i][2], &m_MapData[i][3], &m_MapData[i][4]);
	}
	fclose(fp);

	for (int i = 0;i < 5;i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (m_MapData[i][j] == 2)
			{
				m_EnemyPos = GetVec3Pos(i, j);
			}
		}
	}
	m_nAttackTime = 0;
	m_nMaxgeneration = 3;
	m_DoGeneCount = -1;
	m_beforeDoGeneCount = -2;
	printf("INIT!!\n");
}

vec3 CMainScene::GetIndexPos(vec3 pos)
{
	float Scale = 6;
	pos /= Scale;
	vec3 index;
	index.x = pos.x + 2;
	index.y = 0;
	index.z = pos.z + 2;
	return index;
}

vec3 CMainScene::GetVec3Pos(int y, int x)
{
	float Scale = 6;
	vec3 pos = vec3((x - 2) * Scale, 0, (y - 2) * Scale);
	return pos;
}

void CMainScene::Release()
{
	printf("Release()!!\n");
}

int CMainScene::GetMap(int y, int x)
{
	return m_MapData[y][x];
}

void CMainScene::SetHeroMove(vec3 pos, int hp)
{
	m_HeroPos = pos;
	m_nHeroHP = hp;
}

void CMainScene::SetEnemyInfo(int hp)
{
	m_nEnemyHP = hp;
}

STATE CMainScene::GetState()
{
	STATE a;
	a.Dir = m_EnemyDir;
	a.Pos = m_EnemyPos;
	a.State = m_nState;
	return a;
}

void CMainScene::OnInitGene()
{
	m_Gene.clear();
	for (int i = 0; i < 5;i++)
	{
		GENE a;
		for (int j = 0;j < 5;j++)
		{
			a.g[j] = rand() % 5;
		}
		a.re = 0;
		m_Gene.push_back(a);
	}
}

void CMainScene::OnReleaseGene()
{
	int count = m_Gene.size() * 0.6;

	for (int i = 0; i < count;i++)
	{
		m_Gene.erase(m_Gene.end() - 1);
	}
}

bool CMainScene::GetCheckGene(int i)
{
	vec3 EnemyIndex = GetIndexPos(m_EnemyPos);
	for (int j = 0; j < 5; j++)
	{
		if (m_Gene[i].g[j] == 0)//forward
		{
			EnemyIndex.y--;
		}
		if (m_Gene[i].g[j] == 1)//backward
		{
			EnemyIndex.y++;
		}
		if (m_Gene[i].g[j] == 2)//left
		{
			EnemyIndex.x--;
		}
		if (m_Gene[i].g[j] == 3)//right
		{
			EnemyIndex.x++;
		}

		if (m_Gene[i].g[j] == 4)
		{
			vec3 dir = (m_HeroIndex +(- EnemyIndex)) / 2.0f;

			bool IsWall = false;
			for (int n = 1; n <= 2;n++)
			{
				vec3 pos = EnemyIndex + (dir * n);
				if (m_MapData[(int)pos.y][(int)pos.x] == 1)
				{
					IsWall = true;
				}
			}
			if (IsWall == false)
				m_Gene[i].re++;
		}

		if (EnemyIndex.x >= 5 || EnemyIndex.x < 0)
		{
			m_Gene[i].re = 0;
			return false;
		}
		if (EnemyIndex.y >= 5 || EnemyIndex.y < 0)
		{
			m_Gene[i].re = 0;
			return false;
		}
		if (m_MapData[(int)EnemyIndex.y][(int)EnemyIndex.x] == 1)
		{
			m_Gene[i].re = 0;
			return false;
		}
	}

	m_Gene[i].re += 7.1 - (length(EnemyIndex +(- m_HeroIndex)));
	return true;
}

void CMainScene::onCreateGene()
{
	OnInitGene();
	for (int n = 0;n < m_nMaxgeneration; n++)
	{
		for (int i = 0;i < m_Gene.size();i++)
		{
			GetCheckGene(i);
		}
		sort(m_Gene.begin(), m_Gene.end(), comp);

		OnReleaseGene();
		if (n != m_nMaxgeneration - 1)
			OnMatingGene();
	}
}

void CMainScene::OnMatingGene()
{
	int count = m_Gene.size();
	for (int i = 0;i < count;i++)
	{
		for (int j = 0;j < count; j++)
		{
			if (i != j)
			{
				int pt = rand() % 5 + 1;
				int pt1 = 5 - pt;
				GENE a;
				memcpy(a.g, m_Gene[i].g, sizeof(int) * pt);
				memcpy(&a.g[pt], &m_Gene[j].g[pt1], sizeof(int) * pt1);

				int r = rand() % 2;
				if (r == 1)
				{
					int t = rand() % 5;
					a.g[t] = rand() % 5;
				}
				a.re = 0;
				m_Gene.push_back(a);
			}

		}
	}
}

void CMainScene::FrameMove()
{
	if (m_DoGeneCount != m_beforeDoGeneCount)
	{
		if (m_DoGeneCount >= 5)
		{
			m_DoGeneCount = -1;
		}
		m_HeroIndex = GetIndexPos(m_HeroPos);
		if (m_DoGeneCount == -1)
		{
			printf("CREATE GENE!!\n");
			onCreateGene();
			while (m_Gene[0].re <= 10)
			{
				printf("RE : %0.2f RECREATE GENE!!\n", m_Gene[0].re);
					onCreateGene();
			}
			m_DoGeneCount++;
			printf("CREATE GENERE: %0.2f !! \n", m_Gene[0].re); //0.2f
		}
		if (m_DoGeneCount <= 4)
		{
			m_nState = m_Gene[0].g[m_DoGeneCount];

			vec3 EnemyIndex = GetIndexPos(m_EnemyPos);
			if (m_Gene[0].g[m_DoGeneCount] == 0)
			{
				printf("DoGene %d Forward !! \n", m_DoGeneCount);
				EnemyIndex.y--;
			}
			if (m_Gene[0].g[m_DoGeneCount] == 1)
			{
				printf("DoGene %d Backward !! \n", m_DoGeneCount);
				EnemyIndex.y++;
			}
			if (m_Gene[0].g[m_DoGeneCount] == 2)
			{
				printf("DoGene %d Left !! \n", m_DoGeneCount);
				EnemyIndex.x--;
			}
			if (m_Gene[0].g[m_DoGeneCount] == 3)
			{
				printf("DoGene %d Right !! \n", m_DoGeneCount);
				EnemyIndex.x++;
			}
			if (m_Gene[0].g[m_DoGeneCount] == 4)
			{
				printf("DoGene %d Attack !! \n", m_DoGeneCount);				
			}

			m_EnemyGoalPos = GetVec3Pos(EnemyIndex.y, EnemyIndex.x);
		}
		m_beforeDoGeneCount = m_DoGeneCount;
	}
	if (m_nState >= 0 && m_nState < 4)
	{
		m_EnemyDir = normalize(m_EnemyGoalPos + (-m_EnemyPos));
		m_EnemyPos += m_EnemyDir * 0.1f;
		float len = length(m_EnemyGoalPos +(-m_EnemyPos));
		if (len <= 0.7)
		{
			m_EnemyPos = m_EnemyGoalPos;
			m_DoGeneCount++;
		}
	}

	if (m_nState == 4)
	{
		m_nAttackTime++;
		m_EnemyDir = normalize(m_HeroPos +(-m_EnemyPos));
		if (m_nAttackTime >= 100)
		{
			m_DoGeneCount++;
			m_nAttackTime = 0;
		}
	}
}
