#ifndef GA_KSW_H
#define GA_KSW_H

#define MAPSIZE 13

#define WHITE 1
#define BLACK 2

using namespace std;


struct SW_GENE
{
	char g[8];
	float re;
	int x;
	int y;
};

bool KSW_comp(SW_GENE a, SW_GENE b)
{
	return (a.re > b.re);
}

class cKSW_GA : public cVirtual_GA
{
private:
	float			m_fg;
	vector<SW_GENE>		m_GeneList;
	int				m_nInitGeneCount;
	int				m_nMaxgeneration;
	int				m_nMyID;
	int(*m_ppMap)[MAPSIZE];
	Vector2f			m_vUserPos;


public:

private:
	void	onCreateGene()
	{
		m_GeneList.clear();
		for (int i = 0; i < m_nInitGeneCount; i++)
		{
			SW_GENE a;
			for (int j = 0; j < 8; j++)
			{
				a.g[j] = rand() % 2;

			}

			m_GeneList.push_back(a);
		}

		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				SW_GENE a;
				int dx = m_vUserPos.x + x;
				int dy = m_vUserPos.y + y;

				if (dx <= 0)
					dx = 0;
				if (dy <= 0)
					dy = 0;
				if (dx >= (MAPSIZE - 1))
					dx = MAPSIZE - 1;
				if (dy >= (MAPSIZE - 1))
					dy = MAPSIZE - 1;

				a.x = dx;
				a.y = dy;

				for (int i = 0; i < 4; i++)
				{
					a.g[i] = dx % 2;
					dx = dx / 2;
				}

				for (int i = 4; i < 8; i++)
				{
					a.g[i] = dy % 2;
					dy = dy / 2;
				}
				m_GeneList.push_back(a);
			}
		}
	}

	void	onMatingGene()
	{
		int count = m_GeneList.size();
		for (int i = 0; i < count; i++)
		{
			for (int j = 0; j < count; j++)
			{
				if (i != j)
				{
					int pt = rand() % 8 + 1;
					int pt1 = 8 - pt;
					SW_GENE a;
					memcpy(a.g, m_GeneList[i].g, sizeof(char) * pt);
					memcpy(&a.g[pt], &m_GeneList[j].g[pt1], sizeof(char) * pt1);

					int x = a.g[0] + a.g[1] * 2 + a.g[2] * 4 + a.g[3] * 8;
					int y = a.g[4] + a.g[5] * 2 + a.g[6] * 4 + a.g[7] * 8;

					int r = rand() % 10;
					if (r == 1)
					{
						int t = rand() % 8;
						if (a.g[t] == 0)
							a.g[t] = 1;
						else
							a.g[t] = 0;
					}

					m_GeneList.push_back(a);
				}
			}
		}
	}

	void	 onReleaseGene()
	{
		int count = m_GeneList.size() * 0.8;

		for (int i = 0; i < count; i++)
		{
			m_GeneList.erase(m_GeneList.end() - 1);
		}
	}

	float			GetCheckGene(int id)
	{
		int x = m_GeneList[id].g[0] + m_GeneList[id].g[1] * 2 + m_GeneList[id].g[2] * 4 + m_GeneList[id].g[3] * 8;
		int y = m_GeneList[id].g[4] + m_GeneList[id].g[5] * 2 + m_GeneList[id].g[6] * 4 + m_GeneList[id].g[7] * 8;
		m_GeneList[id].x = x;
		m_GeneList[id].y = y;

		if (y < MAPSIZE && x < MAPSIZE && x >= 0 && y >= 0)
		{
			if (m_ppMap[x][y] == 0)
			{
				Vector2f v = Vector2f(x, y) - m_vUserPos;

				//float p1 = 1.0f-(v.Length()/MAPSIZE);
				float tw = GetCheckWinrate(WHITE, x, y);
				float tb = GetCheckWinrate(BLACK, x, y);
				float g1 = 1.0f - m_fg;
				float t = (tw * m_fg) + (tb * g1);
				float p2 = (t * 1.0);
				m_GeneList[id].re = p2;
				//printf(" tw : %f, tb : %f , t: %f , p2 : %f\n", tw, tb, t, p2);
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
	}
	//==========================================================================================================

	float			GetCheckWinrate(int mid, int x, int y)
	{
		// tScroeMap[i][j] 에서 i의 의미는 가로, 세로, 각 대각선 구분이고 j 는 스코어보드를 의미한다
		int tScore[4] = { 0 };

		//맵 사본 만들기
		int tMap[MAPSIZE][MAPSIZE] = { 0 };
		int tValueMap[MAPSIZE][MAPSIZE] = { 0 };

		for (int i = 0; i < MAPSIZE; i++)
		{
			for (int j = 0; j < MAPSIZE; j++)
			{
				tMap[i][j] = m_ppMap[i][j];
			}
		}
		
		//================================== X 축 점수 더하기=================================
		for (int i = 0; i < MAPSIZE; i++)
		{
			int tValueStack = 0;
			int tStartIndex = MAPSIZE ;

			// + 점수 구하기
			for (int j = 0; j < MAPSIZE; j++)
			{
				if (tMap[i][j] == mid) {
					if (tStartIndex == MAPSIZE) {
						tStartIndex = j;
					}
					if (tValueStack == 0) {
						tValueStack = 2;
					}
					else {
						tValueStack++;
					}
				}
				// 연쇄가 끊어졌을때, 점수 계산
				else if ( tMap[i][j] == 0 &&  tStartIndex != MAPSIZE) {

					if (tStartIndex > 0 ) {
						// 시작 된 곳의 왼 칸이 빈칸일 경우
						if (tMap[i][tStartIndex - 1] == 0) {
							tValueMap[i][tStartIndex - 1] += tValueStack - 1;
						}
						//아닐 경우 빈칸을 찾아서 점수 기록
						else {
							int t = tStartIndex-1;
							while (t >= 0 && tMap[i][t] != 0)
							{
								t--;
							}
							if (t >= 0) {
								tValueMap[i][t] += tValueStack - 1;
							}

						}

					}
					if (j < MAPSIZE - 1 ){
						// 끝난 곳의 오른 칸이 빈칸일 경우
						if (tMap[i][j + 1] == 0) {
							tValueMap[i][j + 1] += tValueStack - 1;
						}
						//아닐 경우 빈칸을 찾아서 점수 기록
						else {
							int t = j + 1;
							while (t < MAPSIZE && tMap[i][t] != 0)
							{
								t++;
							}
							if (t < MAPSIZE) {
								tValueMap[i][t] += tValueStack - 1;
							}

						}



					}
					tStartIndex = MAPSIZE;
					tValueStack = 0;

				}

			}
		}

		//================================== Y 축 점수 더하기=================================
		for (int i = 0; i < MAPSIZE; i++)
		{
			int tValueStack = 0;
			int tStartIndex = MAPSIZE;

			// + 점수 구하기
			for (int j = 0; j < MAPSIZE; j++)
			{
				if (tMap[j][i] == mid ) {
					if (tStartIndex == MAPSIZE) {
						tStartIndex = j;
					}
					if (tValueStack == 0) {
						tValueStack = 2;
					}
					else {
						tValueStack++;
					}
				}
				// 연쇄가 끊어졌을때, 점수 계산
				else if (tMap[j][i] == 0 && tStartIndex != MAPSIZE) {

					if (tStartIndex > 0) {
						// 시작 된 곳의 왼 칸이 빈칸일 경우
						if (tMap[tStartIndex - 1][i] == 0) {
							tValueMap[tStartIndex - 1][i] += tValueStack - 1;
						}
						//아닐 경우 빈칸을 찾아서 점수 기록
						else {
							int t = tStartIndex - 1;
							while (t >= 0 && tMap[t][i] != 0)
							{
								t--;
							}
							if (t >= 0) {
								tValueMap[t][i] += tValueStack - 1;
							}

						}

					}
					if (j < MAPSIZE - 1) {
						// 끝난 곳의 오른 칸이 빈칸일 경우
						if (tMap[j + 1][i] == 0) {
							tValueMap[j + 1][i] += tValueStack - 1;
						}
						//아닐 경우 빈칸을 찾아서 점수 기록
						else {
							int t = j + 1;
							while (t < MAPSIZE && tMap[t][i] != 0)
							{
								t++;
							}
							if (t < MAPSIZE) {
								tValueMap[t][i] += tValueStack - 1;
							}

						}



					}
					tStartIndex = MAPSIZE;
					tValueStack = 0;
				}


			}
		}

		//=============================== / 대각선 점수 더하기================================

		int a = y - x;

		for (int i = 0; i < MAPSIZE; i++)
		{
			int tY = i + a;
			int tValueStack = 0;
			int tStartIndex = MAPSIZE;

			if (tY >= 0 && tY < MAPSIZE) {
				
				//스택 쌓기
				if (tMap[i][tY] == mid)
				{
					if (tStartIndex == MAPSIZE) {
						tStartIndex = i;
					}
					if (tValueStack == 0) {
						tValueStack = 2;
					}
					else {
						tValueStack++;
					}
				}
				//점수 반영
				else if (tMap[i][tY] == 0 && tStartIndex != MAPSIZE) {
					if (tStartIndex > 0 && tStartIndex +a <MAPSIZE) {
						// 시작 된 곳의 왼 칸이 빈칸일 경우
						if (tMap[tStartIndex - 1][tStartIndex + a] == 0) {
							tValueMap[tStartIndex - 1][tStartIndex + a] += tValueStack - 1;
						}
						//아닐 경우 빈칸을 찾아서 점수 기록
						else {
							int t = tStartIndex - 1;
							while (t >= 0 && tMap[t][t+a] != 0)
							{
								t--;
							}
							if (t >= 0) {
								tValueMap[t][t+a] += tValueStack - 1;
							}

						}

					}
					if (i < MAPSIZE - 1) {
						// 끝난 곳의 오른 칸이 빈칸일 경우
						int t = i + 1;
						if (tMap[t][t+a] == 0) {
							tValueMap[t][t+ a] += tValueStack - 1;
						}
						//아닐 경우 빈칸을 찾아서 점수 기록
						else {
							while (t+a < MAPSIZE && tMap[t][t+a] != 0)
							{
								t++;
							}
							if (t < MAPSIZE) {
								tValueMap[t][t+a] += tValueStack - 1;
							}
							
						}



					}
					tStartIndex = MAPSIZE;
					tValueStack = 0;
				}

			}
		}
		


		//================================\대각선 점수 더하기================================
		a = y + x;
		for (int i = 0; i > -MAPSIZE-a; i--)
		{
			int tY = i + a;
			if (tY < MAPSIZE && tY >= 0) {



			}

		}
		
		//================================   p 반환 =========================================

		float p = (float)tValueMap[x][y] / 16.0f;
		return p;

	}
	//==========================================================================================================
public:
	cKSW_GA() {}
	~cKSW_GA() {}

	void Init(int(*data)[MAPSIZE], int myid)
	{
		m_fg = 0.2;

		srand(time(NULL));
		m_ppMap = data;
		m_GeneList.clear();
		m_nMyID = myid;
		m_nMaxgeneration = 4;
		m_nInitGeneCount = 8;
	}

	void* FrameMove(int x, int y)
	{
		m_vUserPos = Vector2f(x, y);
		onCreateGene();
		for (int n = 0; n < m_nMaxgeneration; n++)
		{
			for (int i = 0; i < m_GeneList.size(); i++)
			{
				GetCheckGene(i);
			}
			std::sort(m_GeneList.begin(), m_GeneList.end(), KSW_comp);

			onReleaseGene();
			if (n != (m_nMaxgeneration - 1))
				onMatingGene();

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

