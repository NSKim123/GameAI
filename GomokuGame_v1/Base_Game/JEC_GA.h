#ifndef JEC_GA_H
#define JEC_GA_H

#define MAPSIZE 13
#define WHITE 1
#define BLACK 2
#include <iostream>
#include <cmath>

struct JEC_GENE
{
	char g[8]; // 유전자 배열
	float re; // 우성인진 열성인지
	int x;
	int y;
};

bool JEC_comp(JEC_GENE a, JEC_GENE b) // 유전자 정렬 조건 함수
{
	if (a.re < 0 && b.re < 0)
		return (a.re > b.re);

	else if (a.re < 0 || b.re < 0)
		return (abs(a.re) > abs(b.re));

	else
		return (a.re > b.re);
}


class cJEC_GA : public cVirtual_GA
{
private:
	float m_fg; // 성향
	vector<JEC_GENE> m_GeneList; // 유전자 리스트
	int m_nInitGeneCount; // 처음 생성할 유전자의 개수
	int m_Maxgeneration; // 진행할 세대 수
	int m_nMyID; // 검은돌인지 흰돌인지
	int(*m_ppMap)[MAPSIZE]; // 
	Vector2f m_vUserPos;

public:
	cJEC_GA(){}

	~cJEC_GA(){}

	void Init(int(*data)[MAPSIZE], int myid) override // 초기 세팅
	{
		m_fg = 0.5;
		srand(time(NULL));
		m_ppMap = data;
		m_GeneList.clear();
		m_nMyID = myid;
		m_Maxgeneration = 4;
		m_nInitGeneCount = 8;	
	}

	void* FrameMove(int x, int y) override // Update 실행문
	{
		m_vUserPos = Vector2f(x, y);
		onCreateGene();

		for (int n = 0; n < m_Maxgeneration; n++)
		{
			for (int i = 0; i < m_GeneList.size(); i++)
			{
				GetCheckGene(i);
			}

			sort(m_GeneList.begin(), m_GeneList.end(), JEC_comp);
			onReleaseGene();

			if (n != (m_Maxgeneration - 1))
				onMatingGene();
		}

		Vector2f p = Vector2f(m_GeneList[0].x, m_GeneList[0].y);
		m_GeneList.clear();

		return &p;
	}

	void Release() override // 사용 후 초기화
	{
		m_GeneList.clear();
	}
	

	void onCreateGene() // 유전자 만드는 함수
	{
		m_GeneList.clear();
		for (int i = 0; i < m_nInitGeneCount; i++)
		{
			JEC_GENE a;
			for (int j = 0; j < 8; j++)
			{
				a.g[i] = rand() % 2;
			}

			m_GeneList.push_back(a);
		}

		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				JEC_GENE a;
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

	float GetCheckGene(int id)
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
				float tw = GetCheckWinrate(WHITE, x, y, id);
				float tb = GetCheckWinrate(BLACK, x, y, id);
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

	void onReleaseGene() // 우성 유전자 지우는 함수
	{
		int count = m_GeneList.size() * 0.6;

		for (int i = 0; i < count; i++)
		{
			m_GeneList.erase(m_GeneList.end() - 1);
		}
	}

	void onMatingGene() // 유전자 조합 함수
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
					JEC_GENE a;
					memcpy(a.g, m_GeneList[i].g, sizeof(char) * pt);
					memcpy(&a.g[pt], &m_GeneList[j].g[pt1], sizeof(char) * pt1);

					int r = rand() % 2;
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

	// 유전자를 평가하는 함수
	// 1. 돌 위치에 대한 가산치
	// 2. 연속된 돌에 대한 가산치
	// 3. 상대 돌로 막힌 부분에 대한 역가산치
	// 4. 상대나 자신이 4돌인 경우 다량의 추가 가산치
	// 연산량 아낀다고 for문에 막 우겨넣다보니... 코드가 많이 더럽습니다 ㅠㅠ
	float GetCheckWinrate(int mid, int x, int y, int id)
	{
		int Value[4] = { 0, }; // 각가의 축에서, 돌 위치에 해당하는 가산치를 저장하는 배열


		// X 축
		int Array[13] = { 0, }; // 해당 축에 대한 가산치 저장하는 배열
		int SameCountX = 0;
		int StartPointX = 0;
		bool EnemyBoolX = false;

		for (int i = 0; i < MAPSIZE; i++) // 좌표의 x축을 탐색하며, 돌이 있을 경우 해당하는 가중치 설정
		{
			if (m_ppMap[i][y] == 0) // 돌이 놓여있지 않을 경우 건너뜀
			{
				if (StartPointX - 1 >= 0)
					Array[StartPointX - 1] += SameCountX;

				if (i < MAPSIZE)
					Array[i] += SameCountX;

				SameCountX = 0;
				StartPointX = 0;

				if (EnemyBoolX == true)
					EnemyBoolX = false;

				continue;
			}

			if (m_ppMap[i][y] == mid) // 자신과 같은 색의 돌일 경우 플러스 가산치
			{
				for (int j = 1; j < 5; j++)
				{
					if (i + j < MAPSIZE)
						Array[i + j] += 5 - j;

					if (i - j >= 0)
						Array[i - j] += 5 - j;
				}

				if (StartPointX == 0)
					StartPointX = i;

				SameCountX++;

				if (i == MAPSIZE - 1) // 이번 위치가 맵 끝일 경우, 바로 가산치 적용
					Array[i] += SameCountX;

				if (EnemyBoolX == true)
					EnemyBoolX = false;
			}

			else
			{
				if (StartPointX - 1 >= 0)
					Array[StartPointX - 1] += SameCountX;

				if (i < MAPSIZE)
					Array[i] += SameCountX;

				SameCountX = 0;
				StartPointX = 0;

				if (EnemyBoolX == false)
				{
					for (int j = 1; j < 5; j++)
					{
						if (i + j < MAPSIZE)
							Array[i + j] -= 5 - j;

						if (i - j >= 0)
							Array[i - j] -= 5 - j;
					}
					EnemyBoolX = true;
				}
			}
		}
		Value[0] = Array[x];


		// Y 축
		int Array2[13] = { 0, };
		int SameCountY = 0;
		int StartPointY = 0;
		bool EnemyBoolY = false;

		for (int i = 0; i < MAPSIZE; i++) // 좌표의 x축을 탐색하며, 돌이 있을 경우 해당하는 가중치 설정
		{
			if (m_ppMap[x][i] == 0) // 돌이 놓여있지 않을 경우 건너뜀
			{
				if (StartPointY - 1 >= 0)
					Array2[StartPointY - 1] += SameCountY;

				if (i < MAPSIZE)
					Array2[i] += SameCountY;

				SameCountY = 0;
				StartPointY = 0;

				if (EnemyBoolY == true)
					EnemyBoolY = false;

				continue;
			}

			if (m_ppMap[x][i] == mid) // 자신과 같은 색의 돌일 경우 플러스 가산치
			{
				for (int j = 1; j < 5; j++)
				{
					if (i + j < MAPSIZE)
						Array2[i + j] += 5 - j;

					if (i - j >= 0)
						Array2[i - j] += 5 - j;
				}
				if (StartPointY == 0)
					StartPointY = i;

				SameCountY++;

				if (i == MAPSIZE - 1) // 이번 위치가 맵 끝일 경우, 바로 가산치 적용
					Array2[i] += SameCountY;

				if (EnemyBoolY == true)
					EnemyBoolY = false;
			}

			else
			{
				if (StartPointY - 1 >= 0)
					Array2[StartPointY - 1] += SameCountY;

				if (i < MAPSIZE)
					Array2[i] += SameCountY;

				SameCountY = 0;
				StartPointY = 0;

				if (EnemyBoolY == false)
				{
					for (int j = 1; j < 5; j++)
					{
						if (i + j < MAPSIZE)
							Array2[i + j] -= 5 - j;

						if (i - j >= 0)
							Array2[i - j] -= 5 - j;
					}
					EnemyBoolY = true;
				}
			}
		}

		Value[1] = Array2[y];


		// 대각 축 1
		int Array3[13] = { 0, };
		int SameCountDR = 0;
		int StartPointDR = 0;
		bool EnemyBoolDR = false;

		int a = 0;
		int b = 0;

		if (x >= y)
			a = x - y;
		else
			b = y - x;

		for (int i = 0; i < MAPSIZE; i++) // 좌표의 x축 위치를 탐색하며, 현재 상황의 가중치 설정
		{
			if (a + i < 13 && b + i < 13)
			{
				if (m_ppMap[a + i][b + i] == 0) // 돌이 놓여있지 않을 경우 건너뜀
				{
					if (StartPointDR - 1 >= 0)
						Array3[StartPointDR - 1] += SameCountDR;

					if (i < MAPSIZE)
						Array3[a + i] += SameCountDR;

					SameCountDR = 0;
					StartPointDR = 0;

					if (EnemyBoolDR == true)
						EnemyBoolDR = false;

					continue;
				}

				if (m_ppMap[a + i][b + i] == mid) // 자신과 같은 색의 돌일 경우 플러스 가산치
				{

					for (int j = 1; j <= 4; j++)
					{
						if (a + i + j <= 12)
							Array3[a + i + j] += 5 - j;

						if (a + i - j >= 0)
							Array3[a + i - j] += 5 - j;
					}

					if (StartPointDR == 0)
						StartPointDR = a + i;

					SameCountDR++;

					if (i == MAPSIZE - 1) // 이번 위치가 맵 끝일 경우, 바로 가산치 적용
						Array3[a + i] += SameCountDR;

					if (EnemyBoolDR == true)
						EnemyBoolDR = false;
				}

				else
				{
					if (StartPointDR - 1 >= 0)
						Array3[StartPointDR - 1] += SameCountDR;

					if (i < MAPSIZE)
						Array3[a + i] += SameCountDR;

					SameCountDR = 0;
					StartPointDR = 0;

					if (EnemyBoolDR == false)
					{
						for (int j = 1; j < 5; j++)
						{
							if (a + i + j < MAPSIZE)
								Array3[a + i + j] -= 5 - j;

							if (a + i - j >= 0)
								Array3[a + i - j] -= 5 - j;
						}

						EnemyBoolDR = true;
					}
				}
			}
		}

		Value[2] = Array3[y];


		// 대각 축 2
		int Array4[13] = { 0, };
		int Point[13] = { 0, };
		int SameCountDL = 0;
		int StartPointDL = 0;
		bool EnemyBoolDL = false;

		for (int i = 0; i < 13; i++) // 오른쪽 방향으로 이동하면서 검사
		{
			if (x + i < 13 && y - i >= 0)
			{
				if (m_ppMap[x + i][y - i] == 0) // 돌이 놓여있지 않을 경우 건너뜀
				{
					Point[x + i] = 0;

					if (EnemyBoolDL == true)
						EnemyBoolDL = false;

					continue;
				}

				if (m_ppMap[x + i][y - i] == mid) // 자신과 같은 색의 돌일 경우 플러스 가산치
				{
					for (int j = 1; j <= 4; j++)
					{
						if (x + i + j <= 12)
							Array4[x + i + j] += 5 - j;

						if (x + i - j >= 0)
							Array4[x + i - j] += 5 - j;
					}

					Point[x + i] = 1;

					if (EnemyBoolDL == true)
						EnemyBoolDL = false;
				}

				else // 자신과 다른 색의 돌일 경우
				{
					Point[x + i] = -1;

					if (EnemyBoolDL == false)
					{
						for (int j = 1; j < 5; j++)
						{
							if (x + i + j < MAPSIZE)
								Array4[x + i + j] -= 5 - j;

							if (x + i - j >= 0)
								Array4[x + i - j] -= 5 - j;
						}
						EnemyBoolDL = true;
					}
				}
			}
		}

		for (int i = 0; i < 13; i++) // dhls쪽 방향으로 이동하면서 검사
		{
			if (y + i < 13 && x - i >= 0)
			{
				if (m_ppMap[x - i][y + i] == 0) // 돌이 놓여있지 않을 경우 건너뜀
				{
					Point[x - i] = 0;

					if (EnemyBoolDL == true)
						EnemyBoolDL = false;

					continue;
				}

				if (m_ppMap[x - i][y + i] == mid) // 자신과 같은 색의 돌일 경우 플러스 가산치
				{
					for (int j = 1; j <= 4; j++)
					{
						if (x - i + j <= 12)
						{
							Array4[x - i + j] += 5 - j;
						}

						if (x - i - j >= 0)
						{
							Array4[x - i - j] += 5 - j;
						}
					}

					Point[x - i] = 1;

					if (EnemyBoolDL == true)
						EnemyBoolDL = false;
				}

				else // 자신과 다른 색의 돌일 경우
				{
					Point[x - i] = -1;

					if (EnemyBoolDL == false)
					{
						for (int j = 1; j < 5; j++)
						{
							if (x - i + j < MAPSIZE)
							{
								Array4[x - i + j] -= 5 - j;
							}

							if (x - i - j >= 0)
							{
								Array4[x - i - j] -= 5 - j;
							}
						}
						EnemyBoolDL = true;
					}
				}
			}
		}

		for (int i = 0; i < MAPSIZE; i++)
		{
			if (Point[i] == 0)
			{
				if (StartPointDL - 1 > 0)
					Array4[StartPointDL - 1] += SameCountDL;
				Array4[i] += SameCountDL;

				SameCountDL = 0;
				StartPointDL = 0;
			}

			if (Point[i] == 1)
			{
				if (StartPointDL == 0)
					StartPointDL = i;

				SameCountDL++;
			}

			if (Point[i] == -1)
			{
				if (StartPointDL - 1 > 0)
					Array4[StartPointDL - 1] += SameCountDL;
				Array4[i] += SameCountDL;

				SameCountDL = 0;
				StartPointDL = 0;
			}
		}

		Value[3] = Array4[x];


		// 현재 돌 위치에서 4돌이 생성되는지 검사하는 부분
		int count[4] = { 0, };
		bool left = false;
		bool right = false;

		for (int i = 1; i < 8; i++)
		{
			if (right == true && left == true)
				break;

			int dx1 = x - i;
			int dx2 = x + i;

			if (m_ppMap[dx1][y] == mid && left == false && dx1 >= 0 && dx1 < MAPSIZE)
			{
				count[0]++;
			}
			else
				left = true;
			if (m_ppMap[dx2][y] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE)
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

			if (m_ppMap[x][dy1] == mid && left == false && dy1 >= 0 && dy1 < MAPSIZE)
			{
				count[1]++;
			}
			else
				left = true;
			if (m_ppMap[x][dy2] == mid && right == false && dy2 >= 0 && dy2 < MAPSIZE)
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

			if (m_ppMap[dx1][dy1] == mid && left == false && dx1 >= 0 && dx1 < MAPSIZE && dy1 >= 0 && dy1 < MAPSIZE)
			{
				count[2]++;
			}
			else
				left = true;
			if (m_ppMap[dx2][dy2] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE && dy2 >= 0 && dy2 < MAPSIZE)
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

			if (m_ppMap[dx1][dy1] == mid && left == false && dx1 >= 0 && dx1 < MAPSIZE && dy1 >= 0 && dy1 < MAPSIZE)
			{
				count[3]++;
			}
			else
				left = true;
			if (m_ppMap[dx2][dy2] == mid && right == false && dx2 >= 0 && dx2 < MAPSIZE && dy2 >= 0 && dy2 < MAPSIZE)
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

		if (Max >= 3)
			return 300 + Max; // 임의의 큰 값과 가장 긴 연속된 돌의 수를 더해서 반환


		float p = 0;
		for (int i = 0; i < 4; i++)
		{
			p += Value[i];
		}

		return p;
	}
};

#endif