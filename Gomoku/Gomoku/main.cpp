#include "Include.h"
ExternStruct			g_Extern;
cState*					g_pState =NULL;

void display()
{ 
	

	g_Extern.g_WinSize.x =  glutGet(GLUT_WINDOW_WIDTH);
	g_Extern.g_WinSize.y =  glutGet(GLUT_WINDOW_HEIGHT);

	g_pState->FrameMove();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL,0);

	glViewport(0,0,g_Extern.g_WinSize.x,g_Extern.g_WinSize.y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,g_Extern.g_WinSize.x,0,g_Extern.g_WinSize.y,0.01,1000);
	glEnable(GL_BLEND);

	g_pState->Render();

	glutSwapBuffers(); 
}


void onIdle()
{
	display();
	g_Extern.g_MouseClick =false;
	Sleep(1000.0f/60.0f); 
	
}


void onMouse(int btn ,int state, int x,int y)
{

	if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_Extern.g_Mouse.x=x;
		g_Extern.g_Mouse.y=y;
		g_Extern.g_MouseClick =true;

	}

}

void Release()
{
	g_pState->Release();
	delete g_pState;
	g_pState=NULL;
}


int main()
{


	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); 
	glutInitWindowSize(WINDOWSIZE_WIDTH, WINDOWSIZE_HEIGHT);
	glutCreateWindow("GOMOKU-GA");

	g_Extern.g_WinSize = Vector2f(WINDOWSIZE_WIDTH, WINDOWSIZE_HEIGHT);
	g_Extern.g_Mouse =Vector2f(-1,-1);
	g_Extern.g_MouseClick =false;

	glEnable(GL_TEXTURE_2D);
	g_pState =new cState();
	g_pState->Init();
	glutDisplayFunc(display); 

	glutMouseFunc(onMouse); 
	glutIdleFunc(onIdle);
	atexit(Release);
	glutMainLoop(); 


	return 0;



} 
