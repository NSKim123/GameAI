#ifndef STAGE_H__
#define STAGE_H__

class cStage
{
private:
	int					m_nturnid;
	int					m_nMap[MAPSIZE][MAPSIZE];
	Sprite				m_Map;
	Sprite				m_dot[MAPSIZE][MAPSIZE];
	cVirtual_GA*				m_pGAW;
	cVirtual_GA*				m_pGAB;
	int					m_nlposx;
	int					m_nlposy;

	bool					m_bEnd;
	int					m_nFramenum;
public:

private:
	void onDrawLine(Vector2f p1, Vector2f p2, Color4f c, float width)
	{
		Vector2f size = Vector2f(g_Extern.WINDOWSIZE_WIDTH, g_Extern.WINDOWSIZE_HEIGHT);
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_NOTEQUAL, 0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		Vector2f pp1 = p1 / size * 2 + Vector2f(-1, -1);
		Vector2f pp2 = p2 / size * 2 + Vector2f(-1, -1);
		pp1 = Vector2f(pp1.x, -pp1.y);
		pp2 = Vector2f(pp2.x, -pp2.y);
		glColor4f(c.r, c.g, c.b, c.a);

		glLineWidth((GLfloat)width);
		glBegin(GL_LINES);
		glVertex2f(pp1.x, pp1.y);
		glVertex2f(pp2.x, pp2.y);
		glEnd();

		glPopMatrix();
		glColor4f(1, 1, 1, 1);
	}
	void onDrawMap()
	{
		m_Map.Render();
		int pw = (MAPSIZE + 1);
		float dx = g_Extern.WINDOWSIZE_WIDTH / pw;
		float dy = g_Extern.WINDOWSIZE_HEIGHT / pw;
		for (int i = 0; i < (pw - 1); i++)
		{

			Vector2f p1 = Vector2f(dx * (i + 1), dy);
			Vector2f p2 = Vector2f(dx * (i + 1), g_Extern.WINDOWSIZE_HEIGHT - dy);
			Vector2f p3 = Vector2f(dx, dy * (i + 1));
			Vector2f p4 = Vector2f(g_Extern.WINDOWSIZE_WIDTH - dx, dy * (i + 1));
			onDrawLine(p1, p2, Color4f(0, 0, 0, 1), 3);
			onDrawLine(p3, p4, Color4f(0, 0, 0, 1), 3);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_NOTEQUAL, 0);

		glViewport(0, 0, g_Extern.WINDOWSIZE_WIDTH, g_Extern.WINDOWSIZE_HEIGHT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, g_Extern.WINDOWSIZE_WIDTH, 0, g_Extern.WINDOWSIZE_HEIGHT, 0.01, 1000);
		glEnable(GL_BLEND);

		for (int i = 0; i < (pw - 1); i++)
		{

			Vector2f p1 = Vector2f(dx * (i + 1), dy);
			Vector2f p2 = Vector2f(dx * (i + 1), g_Extern.WINDOWSIZE_HEIGHT - dy);
			Vector2f p3 = Vector2f(dx, dy * (i + 1));
			Vector2f p4 = Vector2f(g_Extern.WINDOWSIZE_WIDTH - dx, dy * (i + 1));



		}

		for (int x = 0; x < MAPSIZE; x++)
		{
			for (int y = 0; y < MAPSIZE; y++)
			{
				m_dot[x][y].Render();
			}
		}
	}

	bool IsWin(int mid, int x, int y)
	{
		int count[4] = { 0, };
		bool left = false;
		bool right = false;

		for (int i = 1; i < 8; i++)
		{
			if (right == true && left == true)
				break;

			int dx1 = x - i;
			int dx2 = x + i;

			if (m_nMap[dx1][y] == mid && left == false && dx1 >= 0 && dx1 < MAPSIZE)
			{
				count[0]++;
			}
			else
				left = true;
			if (m_nMap[dx2][y] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE)
			{
				count[0]++;
			}
			else
				right = true;
		}

		left = false;
		right = false;
		for (int i = 1; i < 8; i++)
		{
			if (right == true && left == true)
				break;

			int dy1 = y - i;
			int dy2 = y + i;

			if (m_nMap[x][dy1] == mid && left == false && dy1 >= 0 && dy1 < MAPSIZE)
			{
				count[1]++;
			}
			else
				left = true;
			if (m_nMap[x][dy2] == mid && right == false && dy2 >= 0 && dy2 < MAPSIZE)
			{
				count[1]++;
			}
			else
				right = true;
		}

		left = false;
		right = false;
		for (int i = 1; i < 8; i++)
		{
			if (right == true && left == true)
				break;

			int dx1 = x + i;
			int dy1 = y - i;

			int dx2 = x - i;
			int dy2 = y + i;

			if (m_nMap[dx1][dy1] == mid && left == false && dx1 >= 0 && dx1 < MAPSIZE && dy1 >= 0 && dy1 < MAPSIZE)
			{
				count[2]++;
			}
			else
				left = true;
			if (m_nMap[dx2][dy2] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE && dy2 >= 0 && dy2 < MAPSIZE)
			{
				count[2]++;
			}
			else
				right = true;
		}

		left = false;
		right = false;
		for (int i = 1; i < 8; i++)
		{
			if (right == true && left == true)
				break;

			int dx1 = x + i;
			int dy1 = y + i;

			int dx2 = x - i;
			int dy2 = y - i;

			if (m_nMap[dx1][dy1] == mid && left == false && dx1 >= 0 && dx1 < MAPSIZE && dy1 >= 0 && dy1 < MAPSIZE)
			{
				count[3]++;
			}
			else
				left = true;
			if (m_nMap[dx2][dy2] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE && dy2 >= 0 && dy2 < MAPSIZE)
			{
				count[3]++;
			}
			else
				right = true;
		}


		int Max = 0;
		for (int i = 0; i < 4; i++)
		{
			if (Max <= count[i])
				Max = count[i];
		}

		if (Max >= 4)
			return true;
		else
			return false;
	}

public:
	cStage() {}
	~cStage() {}

	void init()
	{
		m_bEnd = false;
		m_nlposx = 0;
		m_nlposy = 0;
		memset(m_nMap, 0, sizeof(int) * MAPSIZE * MAPSIZE);
		m_Map.init();
		m_Map.m_size = Vector2f(g_Extern.WINDOWSIZE_WIDTH, g_Extern.WINDOWSIZE_HEIGHT);
		m_Map.m_Color = Color4f(220.0f / 255.0f, 179.0f / 255.0f, 92.0f / 255.0f, 1);
		m_nturnid = 0;
		for (int x = 0; x < MAPSIZE; x++)
		{
			for (int y = 0; y < MAPSIZE; y++)
			{
				m_nMap[x][y] = 0;
				m_dot[x][y].init("Image/cir.png");
				m_dot[x][y].m_size = Vector2f(8, 8);
				m_dot[x][y].m_Color = Color4f(0, 0, 0, 1);
			}
		}
		m_nMap[6][6] = BLACK;
		m_nturnid++;
		m_nFramenum = 0;
		m_pGAW = new cGA ();
		m_pGAW->Init(m_nMap, WHITE, 0.2f);

		m_pGAB = new  cGA();
		m_pGAB->Init(m_nMap, BLACK, 0.8f);
		m_nlposx = 6;
		m_nlposy = 6;
	}

	void Release()
	{

		m_pGAW->Release();
		delete m_pGAW;

		m_pGAB->Release();
		delete m_pGAB;

		m_Map.Release();

		for (int x = 0; x < MAPSIZE; x++)
		{
			for (int y = 0; y < MAPSIZE; y++)
			{
				m_dot[x][y].Release();
			}
		}
	}

	void FrameMove()
	{
		int pw = (MAPSIZE + 1);
		float dx = g_Extern.WINDOWSIZE_WIDTH / pw;
		float dy = g_Extern.WINDOWSIZE_HEIGHT / pw;
		if (m_bEnd == false)
		{
			m_nFramenum = 0;
			if (m_bEnd == false)
			{
				if (m_nturnid % 2 == WHITE)
				{
					DWORD ftime = GetTickCount();
					Vector2f* at = (Vector2f*)(m_pGAW->FrameMove(m_nlposx, m_nlposy));
					Vector2f pga = *at;
					m_nMap[(int)pga.x][(int)pga.y] = WHITE;
					DWORD ltime = GetTickCount();
					float t = (ltime - ftime) / 1000.0f;
					printf("%f\n", t);
					if (IsWin(WHITE, (int)pga.x, (int)pga.y))
					{
						printf("WHITE WIN!!\n");

						m_bEnd = true;
					}
					m_nturnid++;
				}
				else
				{
					DWORD ftime = GetTickCount();
					Vector2f* at = (Vector2f*)(m_pGAB->FrameMove(m_nlposx, m_nlposy));
					Vector2f pga = *at;
					m_nMap[(int)pga.x][(int)pga.y] = BLACK;
					DWORD ltime = GetTickCount();
					float t = (ltime - ftime) / 1000.0f;
					printf("%f\n", t);
					if (IsWin(BLACK, (int)pga.x, (int)pga.y))
					{
						printf("BLACK WIN!!\n");

						m_bEnd = true;
					}
					m_nturnid++;

				}
			}
		}
		else
		{
			g_Extern.g_MouseClick = false;
			if (m_nFramenum == 0)
				printf("Again ? <y/n> \n");
			else
			{

				char t;
				scanf("%c", &t);
				if (t == 'y')
				{
					Release();
					init();
					system("cls");
				}
				if (t == 'n')
				{
					exit(0);
				}
			}
			m_nFramenum++;
		}




	}

	void Render()
	{
		int pw = (MAPSIZE + 1);
		float dx = g_Extern.WINDOWSIZE_WIDTH / pw;
		float dy = g_Extern.WINDOWSIZE_HEIGHT / pw;
		for (int x = 0; x < MAPSIZE; x++)
		{
			for (int y = 0; y < MAPSIZE; y++)
			{
				m_dot[x][y].m_pos = Vector2f(dx * (x + 1), dy * (y + 1)) - m_dot[x][y].m_size / 2;
				if (m_nMap[x][y] == WHITE)
				{
					m_dot[x][y].m_Color = Color4f(1, 1, 1, 1);
					m_dot[x][y].m_size = Vector2f(dx * 0.8, dy * 0.8);
				}

				if (m_nMap[x][y] == BLACK)
				{
					m_dot[x][y].m_Color = Color4f(0, 0, 0, 1);
					m_dot[x][y].m_size = Vector2f(dx * 0.8, dy * 0.8);
				}

				if (m_nMap[x][y] == 0)
				{
					m_dot[x][y].m_size = Vector2f(8, 8);
					m_dot[x][y].m_Color = Color4f(0, 0, 0, 1);
				}
			}
		}

		m_Map.m_size = Vector2f(g_Extern.WINDOWSIZE_WIDTH, g_Extern.WINDOWSIZE_HEIGHT);
		glClearColor(0.5, 0.5, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		onDrawMap();

	}

};
#endif