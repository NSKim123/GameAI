#ifndef INCLUDE_H__
#define INCLUDE_H__

#include<stdlib.h>
#include"glut.h"
#pragma comment(lib,"glut32.lib") 

#include <math.h>
#include <stdio.h>
#include <Windows.h>


#pragma warning( disable : 4996 )
#include <tchar.h>
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <algorithm>
using namespace std;
#define WINDOWSIZE_WIDTH 600 
#define WINDOWSIZE_HEIGHT 600 

#define MAPSIZE 13
#define PI 3.14

#define WHITE 1
#define BLACK 2

inline int KeyDown( int vk )
{
	return ((GetAsyncKeyState(vk) & 0x8000 ) ? 1 : 0 ) ;
}

inline int KeyUp( int vk )
{
	return ((GetAsyncKeyState(vk) & 0x8000 ) ? 0 : 1 ) ;
}

#include "vector.h"

struct ExternStruct
{
	Vector2f		g_Mouse;
	Vector2f		g_WinSize;
	bool			g_MouseClick;
};
extern ExternStruct g_Extern;	

#include "Texture.h"
#include "Sprite.h"

#include "GA\GA.h"
#include "State.h"
#endif