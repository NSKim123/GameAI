#include<stdlib.h>
#include "OpenGL\InitOpengl.h"
#include "OpenCV\InitOpenCV.h"

#include <math.h>
#include <stdio.h>
#include <Windows.h>

#include "vector.h"
struct ExternStruct
{
	int WINDOWSIZE_WIDTH ;
	int WINDOWSIZE_HEIGHT;
	
	Vector2f	g_Mouse;
	bool	g_MouseClick;
};
extern ExternStruct g_Extern;


#include "Texture.h"
#include "Sprite.h"

#include <time.h>
#include <algorithm>

#define PI 3.14

static int KeyDown( int vk )
{

	return ((GetAsyncKeyState(vk) & 0x8000 ) ? 1 : 0 ) ;

}

static int KeyUp( int vk )
{

	return ((GetAsyncKeyState(vk) & 0x8000 ) ? 0 : 1 ) ;

}

#include <vector>
using namespace std;

#include "Vritual_GA.h"
#include "GA.h"


#include "Stage.h"