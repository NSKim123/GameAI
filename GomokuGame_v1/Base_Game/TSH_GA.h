#ifndef TSH_GA_H
#define TSH_GA_H

#define MAPSIZE 13

#define WHITE 1
#define BLACK 2

struct TSH_GENE
{
	//char g[8];
	// 8 --> 24
	char g[24];
	float re;
	int x;
	int y;
};

bool TSH_comp(TSH_GENE a, TSH_GENE b)
{
	return (a.re > b.re);
}


class cTSH_GA : public cVirtual_GA
{
private:
	float	m_fg;
	vector<TSH_GENE> m_GeneList;
	int		m_nInitGeneCount;
	int		m_nMaxgeneration;
	int		m_nMyID;
	int(*m_ppMap)[MAPSIZE];
	Vector2f	m_vUserPos;


public:

private:
	void			onCreateGene()
	{
		m_GeneList.clear();
		for (int i = 0; i < m_nInitGeneCount; ++i)
		{
			TSH_GENE a;
			//for (int j = 0; j < 8; ++j)
			for (int j = 0; j < 24; ++j)
			{
				a.g[i] = rand() % 2;
			}
			m_GeneList.push_back(a);
		}

		// 상대 지역 고려
		//for (int x = -1; x <= 1; ++x)
		for (int x = -2; x <= 2; ++x)
		{
			//for (int y = -1; y <= 1; ++y)
			for (int y = -2; y <= 2; ++y)
			{
				TSH_GENE a;
				int dx = m_vUserPos.x + x;
				int dy = m_vUserPos.y + y;

				//오목판 나가는거 예외처리
				if (dx <= 0)
				{
					dx = 0;
				}

				if (dy <= 0)
				{
					dy = 0;
				}

				if (dx >= (MAPSIZE - 1))
				{
					dx = (MAPSIZE - 1);
				}

				if (dy >= (MAPSIZE - 1))
				{
					dy = (MAPSIZE - 1);
				}

				a.x = dx;
				a.y = dy;

				//for (int i = 0; i < 4; ++i)
				for (int i = 0; i < 12; ++i)
				{
					a.g[i] = dx % 2;
					dx = dx / 2;
				}

				//for (int i = 4; i < 8; ++i)
				for (int i = 12; i < 24; ++i)
				{
					a.g[i] = dy % 2;
					dy = dy / 2;
				}
				m_GeneList.push_back(a);
			}
		}
	}

	void			onMatingGene()
	{
		int count = m_GeneList.size();
		for (int i = 0; i < count; ++i)
		{
			for (int j = 0; j < count; ++j)
			{
				if (i != j)
				{
					int pt = rand() % 24 + 1;
					int pt1 = 24 - pt;
					TSH_GENE a;

					//for문돌리는거보다 빠름
					memcpy(a.g, m_GeneList[i].g, sizeof(char) * pt);
					memcpy(&a.g[pt], &m_GeneList[j].g[pt1], sizeof(char) * pt1);

					//ii) 기존 코드
					/*int x = a.g[0] + a.g[1] * 2 + a.g[2] * 4 + a.g[3] * 8;
					int y = a.g[4] + a.g[5] * 2 + a.g[6] * 4 + a.g[7] * 8;*/

					// 24개의 정보값을 각각의 숫자로 대응
					int x = a.g[0] + a.g[1] * 2 + a.g[2] * 4 + a.g[3] * 8 + a.g[4] * 16 + a.g[5] * 32 + a.g[6] * 64 + a.g[7] * 128 + a.g[8] * 256 + a.g[9] * 512 + a.g[10] * 1024 + a.g[11] * 2048;
					int y = a.g[12] + a.g[13] * 2 + a.g[14] * 4 + a.g[15] * 8 + a.g[16] * 16 + a.g[17] * 32 + a.g[18] * 64 + a.g[19] * 128 + a.g[20] * 256 + a.g[21] * 512 + a.g[22] * 1024 + a.g[23] * 2048;

					//돌연변이
					int r = rand() % 2;
					if (r == 1)
					{
						//int t = rand() % 8;
						int t = rand() % 24;
						if (a.g[t] == 0)
						{
							a.g[t] = 1;
						}
						else
						{
							a.g[t] = 0;
						}

						m_GeneList.push_back(a);
					}
				}
			}
		}
	}
	void			onReleaseGene()
	{
		int count = m_GeneList.size() * 0.6;
		for (int i = 0; i < count; ++i)
		{
			m_GeneList.erase(m_GeneList.end() - 1);
		}
	}


	//new function

	float			GetCheckGene(int id)
	{
		/*int x = m_GeneList[id].g[0] + m_GeneList[id].g[1] * 2 + m_GeneList[id].g[2] * 4 + m_GeneList[id].g[3] * 8;
		int y = m_GeneList[id].g[4] + m_GeneList[id].g[5] * 2 + m_GeneList[id].g[6] * 4 + m_GeneList[id].g[7] * 8;*/
		int x = m_GeneList[id].g[0] + m_GeneList[id].g[1] * 2 + m_GeneList[id].g[2] * 4 + m_GeneList[id].g[3] * 8 + m_GeneList[id].g[4] * 16 + m_GeneList[id].g[5] * 32 + m_GeneList[id].g[6] * 64 + m_GeneList[id].g[7] * 128 + m_GeneList[id].g[8] * 256 + m_GeneList[id].g[9] * 512 + m_GeneList[id].g[10] * 1024 + m_GeneList[id].g[11] * 2048;
		int y = m_GeneList[id].g[12] + m_GeneList[id].g[13] * 2 + m_GeneList[id].g[14] * 4 + m_GeneList[id].g[15] * 8 + m_GeneList[id].g[16] * 16 + m_GeneList[id].g[17] * 32 + m_GeneList[id].g[18] * 64 + m_GeneList[id].g[19] * 128 + m_GeneList[id].g[20] * 256 + m_GeneList[id].g[21] * 512 + m_GeneList[id].g[22] * 1024 + m_GeneList[id].g[23] * 2048;

		m_GeneList[id].x = x;
		m_GeneList[id].y = y;

		if (y < MAPSIZE && x < MAPSIZE && x >= 0 && y >= 0)
		{
			if (m_ppMap[x][y] == 0)
			{
				Vector2f v = Vector2f(x, y) - m_vUserPos;
				float tw = GetCheckWinrate(WHITE, x, y);
				float tb = GetCheckWinrate(BLACK, x, y);
				float g1 = 1.0f - m_fg;
				float p2 = (tw * m_fg) + (tb * g1);
				m_GeneList[id].re = p2;
				return p2;
			}
			m_GeneList[id].re = 0;
			return 0;
		}
		else
		{
			m_GeneList[id].re = 0;
			return 0;
		}
		return 0;
	}

	float			GetCheckWinrate(int mid, int x, int y)
	{

		int count[4] = { 0 };
		bool left = false;
		bool right = false;


		for (int i = 1; i < 8; ++i)
		{
			if (right == true && left == true)
			{
				break;
			}

			int dx1 = x - i;
			int dx2 = x + i;

			if (m_ppMap[dx1][y] == mid && left == false && dx1 >= 0 && dx1 < MAPSIZE)
			{
				++count[0];
			}
			else
			{
				left = true;
			}

			if (m_ppMap[dx2][y] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE)
			{
				++count[0];
			}
			else
			{
				right = true;
			}
			left = false;
		}

		left = false;
		right = false;

		for (int i = 1; i < 8; ++i)
		{
			if (right == true && left == true)
			{
				break;
			}

			int dy1 = y - i;
			int dy2 = y + i;

			if (m_ppMap[x][dy1] == mid && left == false && dy1 >= 0 && dy1 < MAPSIZE)
			{
				++count[1];
			}
			else
			{
				left = true;
			}

			if (m_ppMap[x][dy2] == mid && right == false && dy2 >= 0 && dy2 < MAPSIZE)
			{
				++count[1];
			}
			else
			{
				right = true;
			}
		}

		left = false;
		right = false;

		for (int i = 1; i < 8; ++i)
		{
			if (right == true && left == true)
			{
				break;
			}

			int dx1 = x + i;
			int dy1 = y - i;

			int dx2 = x - i;
			int dy2 = y + i;

			if (m_ppMap[dx1][dy1] == mid && left == false && dy1 >= 0 && dy1 < MAPSIZE && dy1 >= 0 && dy1 < MAPSIZE)
			{
				++count[2];
			}
			else
			{
				left = true;
			}

			if (m_ppMap[dx2][dy2] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE && dy2 >= 0 && dy2 < MAPSIZE)
			{
				++count[2];
			}
			else
			{
				right = true;
			}
		}

		left = false;
		right = false;

		for (int i = 1; i < 8; ++i)
		{
			if (right == true && left == true)
			{
				break;
			}

			int dx1 = x + i;
			int dy1 = y + i;

			int dx2 = x - i;
			int dy2 = y - i;

			if (m_ppMap[dx1][dy1] == mid && left == false && dx1 >= 0 && dx1 < MAPSIZE && dy1 >= 0 && dy1 < MAPSIZE)
			{
				++count[3];
			}
			else
			{
				left = true;
			}

			if (m_ppMap[dx2][dy2] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE && dy2 >= 0 && dy2 < MAPSIZE)
			{
				++count[3];
			}
			else
			{
				right = true;
			}
		}

		int Max = 0;
		for (int i = 0; i < 4; ++i)
		{
			if (Max <= count[i])
			{
				Max = count[i];
			}
		}

		float p = (float)Max / (float)4.0f;
		return p;
	}


public:
	cTSH_GA() {}
	~cTSH_GA() {}

	void				Init(int(*data)[MAPSIZE], int myid)
	{
		m_fg = 0.5f;
		srand(time(NULL));
		m_ppMap = data;
		m_GeneList.clear();
		m_nMaxgeneration = 4;
		m_nInitGeneCount = 8;
		m_nMyID = myid;
	}
	void* FrameMove(int x, int y)
	{
		m_vUserPos = Vector2f(x, y);

		onCreateGene();
		for (int n = 0; n < m_nMaxgeneration; ++n)
		{
			for (int i = 0; i < m_GeneList.size(); i++)
			{
				GetCheckGene(i);
			}
			sort(m_GeneList.begin(), m_GeneList.end(), TSH_comp);
			onReleaseGene();
			if (n != (m_nMaxgeneration - 1))
			{
				onMatingGene();
			}
		}

		Vector2f p = Vector2f(m_GeneList[0].x, m_GeneList[0].y);
		m_GeneList.clear();

		return &p;
	}

	void Release()
	{
		m_GeneList.clear();
	}

};

#endif