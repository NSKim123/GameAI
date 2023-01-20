#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <process.h>
#include <math.h>
#include "MainScene.h"
#pragma warning(disable:4996)
#define UNITY_INTERFACE_API  __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)

static CMainScene* m_pMainScene = NULL;

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API Init()
{
	AllocConsole();
	freopen("conout$", "wt", stdout);
	if (m_pMainScene != NULL)
		delete (m_pMainScene);

	m_pMainScene = NULL;
	m_pMainScene = new CMainScene;
	m_pMainScene->init();
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

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetState()
{
	if (m_pMainScene != NULL)
	{
		m_pMainScene->FrameMove();
		return m_pMainScene->GetState();
	}
	else
		return 0;
}