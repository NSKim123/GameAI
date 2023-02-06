#include "Include.h"
ExternStruct g_Extern;

cStage*		m_pStage =NULL;



////////////////////////////////////////////////////////////

int timer = 0;

void init()
{
	g_Extern.g_Mouse =Vector2f(-1,-1);
	g_Extern.g_MouseClick=false;
	if(m_pStage==NULL)
	{
		m_pStage = new cStage();
		m_pStage->init();
	}


}

void FrameMove()
{
	if(m_pStage!=NULL)
	{
		m_pStage->FrameMove();
	}
}

void Render()
{
	if(m_pStage!=NULL)
	{
		m_pStage->Render();
	}
}

void Release()
{
	if(m_pStage!=NULL)
	{
		m_pStage->Release();
		delete m_pStage;
		m_pStage=NULL;
	}
}

/////////////////////////////////////////////////////////////

void display()
{ 
	if (timer >= 50)
	{
		FrameMove();
		timer = 0;
	}

	timer++;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0,0,g_Extern.WINDOWSIZE_WIDTH,g_Extern.WINDOWSIZE_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,g_Extern.WINDOWSIZE_WIDTH,0,g_Extern.WINDOWSIZE_HEIGHT,0.01,1000);


	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	Render();
	glutSwapBuffers(); 

}


void onIdle()
{
	g_Extern.WINDOWSIZE_WIDTH= glutGet(GLUT_WINDOW_WIDTH);
	g_Extern.WINDOWSIZE_HEIGHT= glutGet(GLUT_WINDOW_HEIGHT);

		display();
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



int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	g_Extern.WINDOWSIZE_WIDTH=600;
	g_Extern.WINDOWSIZE_HEIGHT=600;

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); 
	glutInitWindowSize(g_Extern.WINDOWSIZE_WIDTH, g_Extern.WINDOWSIZE_HEIGHT);
	glutCreateWindow("GAME");

	glEnable(GL_TEXTURE_2D);
	init();
	glutDisplayFunc(display); 
	atexit(Release);
	glutMouseFunc(onMouse); 
	glutIdleFunc(onIdle);
	glutMainLoop(); 


	return 0;



} 
