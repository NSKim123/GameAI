// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <process.h>
#include <math.h>
#include <vector>
#include "vec.h"
#include "MainScene.h"

#define UNITY_INTERFACE_API __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)

static CMainScene* m_pMainScene = NULL;


// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.


extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API Init()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	AllocConsole();
	freopen("count$", "wt", stdout);

	if (m_pMainScene != NULL)
		delete m_pMainScene;
		
	m_pMainScene = NULL;
	m_pMainScene = new CMainScene();
	m_pMainScene->Init();


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

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onSetHeroInfo(vec3 pos, int hp)
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->SetHeroInfo(pos,hp);
	}
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onSetEnemyInfo(int hp)
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->SetEnemyInfo(hp);
	}
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onFrameMove(int hp)
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->FrameMove();
	}
}

extern "C" STATE UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onGetState(int hp)
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->GetState();
	}
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API onGetMapData(int hp)
{
	if (m_pMainScene != NULL)
	{
		return m_pMainScene->GetMap(x, y);
	}
	else 
		return 0;
}