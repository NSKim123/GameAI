
#include "pch.h"
#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <process.h>
#include <math.h>
#include <vector>
#include "vec.h"
#include "MainScene.h"
#pragma warning(disable::4996)
#define UNITY_INTERFACE_API __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)

static CMainScene* m_pMainScene = NULL;


extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API Init()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	AllocConsole();
	freopen("count$", "wt", stdout);

	if (m_pMainScene != NULL)
		delete m_pMainScene;

	m_pMainScene = NULL;
	m_pMainScene = new CMainScene();
	//m_pMainScene->init(Herocount, Enemycount);

}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onRelease()
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->Release();
		delete m_pMainScene;
		m_pMainScene = NULL;
	}
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onSetHeroInfo(int id, int hp)
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->SetHeroInfo(id, hp);
	}
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onSetEnemyInfo(int id, int hp)
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->SetEnemyInfo(id, hp);
	}
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onFrameMove(int hp)
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->FrameMove();
	}
}

extern "C" TANK UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onGetHeroInfo(int id)
{
	if (m_pMainScene != NULL)
	{
		return m_pMainScene->GetHeroInfo(id);
	}
}

extern "C" TANK UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onGetEnemyInfo(int id)
{
	if (m_pMainScene != NULL)
	{
		return m_pMainScene->GetEnemyInfo(id);
	}
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onGetHeroCount()
{
	if (m_pMainScene != NULL)
	{
		return m_pMainScene->GetHeroCount();
	}	
	return 0;
}
extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onGetEnemyCount()
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->GetEnemyCount();
	}
	return 0;
}