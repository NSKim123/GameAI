#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <process.h>
#include <math.h>
#include <omp.h>
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

	int t = -10000;

	int thread_num = omp_get_max_threads() * 0.5;
	if (thread_num <= 0)
		thread_num = 1;
	omp_set_num_threads(thread_num);

	int tid = 0;
#pragma omp parallel private(tid)  //병렬 프로세스
	{
		tid = omp_get_thread_num();
		for (int i = tid; i < 100000000; i+= thread_num)
		{
			int p = (int)cos(i * 100.0f) * 10 + (int)sin(i * 100.0f) * 10;
			t += p;
		}
	}

	printf("Init");
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