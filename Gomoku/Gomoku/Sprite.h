#include <math.h>

struct Sprite
{
	
	Vector2f m_pos;
	Vector2f m_size;
	GLuint	m_Texid;
	Texture		m_Tex;
	Color4f		m_Color;
	void init(const char *name)
	{
		m_pos =Vector2f(0,0);
		m_size = Vector2f(0,0);
		m_Tex.LoadImage(name);
		m_Texid = NULL;
		m_Texid =*m_Tex.GetTexture();
		m_size = m_Tex.GetSize();

		m_Color =Color4f(1,1,1,1);
	}

	void init()
	{
		m_pos =Vector2f(0,0);
		m_size = Vector2f(100,100);
		m_Texid= NULL;
		m_Color =Color4f(1,1,1,1);
	}

	void Release()
	{
		if(m_Texid!=NULL)
		{
			m_Tex.Release();
			m_Texid=NULL;
		}
	}



	void DrawBox(float size)
	{
		static GLfloat n[6][3] =
		{
			{-1.0, 0.0, 0.0},
			{0.0, 1.0, 0.0},
			{1.0, 0.0, 0.0},
			{0.0, -1.0, 0.0},
			{0.0, 0.0, 1.0},
			{0.0, 0.0, -1.0}
		};
		static GLint faces[6][4] =
		{
			{0, 1, 2, 3},
			{3, 2, 6, 7},
			{7, 6, 5, 4},
			{4, 5, 1, 0},
			{5, 6, 2, 1},
			{7, 4, 0, 3}
		};
		GLfloat v[8][3];


		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

		for (int i = 0; i <6; i++) 
		{
			glBegin(GL_QUADS);
			glNormal3fv(&n[i][0]);
			glTexCoord2f(0,1);
			glVertex3fv(&v[faces[i][0]][0]);
			glTexCoord2f(0,0);
			glVertex3fv(&v[faces[i][1]][0]);
			glTexCoord2f(1,0);
			glVertex3fv(&v[faces[i][2]][0]);
			glTexCoord2f(1,1);
			glVertex3fv(&v[faces[i][3]][0]);
			glEnd();
		}
	}

	void Render()
	{

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D,m_Texid);
		glColor4f(m_Color.r,m_Color.g,m_Color.b,m_Color.a);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef((m_size.x/2)+m_pos.x,g_Extern.g_WinSize.y-m_pos.y-(m_size.y/2),0);
		glScalef(-m_size.x,m_size.y,1);
		DrawBox(1);
		glBindTexture(GL_TEXTURE_2D,0);
		glPopMatrix();
	}
};