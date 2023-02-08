#ifndef KNS_GA_H
#define KNS_GA_H

#define MAPSIZE 13

#define WHITE 1
#define BLACK 2


struct KNS_GENE
{
	char g[8];
	float re;
	int x;
	int y;  
};

bool KNS_comp(KNS_GENE a, KNS_GENE b)
{
	return (a.re > b.re);
}

class cKNS_GA : public cVirtual_GA
{
private:
	float			m_fg;
	vector<KNS_GENE>		m_GeneList;
	int				m_nInitGeneCount;
	int				m_nMaxgeneration;
	int				m_nMyID;
	int				(*m_ppMap)[MAPSIZE];
	Vector2f			m_vUserPos;


public:

private:
	void	onCreateGene()
	{
		m_GeneList.clear();
		for(int i=0; i<m_nInitGeneCount;i++)
		{
			KNS_GENE a;
			for(int j=0; j<8; j++)
			{
				a.g[j]=rand()%2;

			}

			m_GeneList.push_back(a);
		}

		for(int x=-1;x<=1;x++)
		{
			for(int y=-1;y<=1;y++)
			{
				KNS_GENE a;
				int dx = m_vUserPos.x+x;
				int dy = m_vUserPos.y+y;

				if(dx<=0)
					dx=0;
				if(dy<=0)
					dy=0;
				if(dx>=(MAPSIZE-1))
					dx= MAPSIZE-1;
				if(dy>=(MAPSIZE-1))
					dy= MAPSIZE-1;

				a.x=dx;
				a.y=dy;

				for(int i=0; i<4;i++)
				{
					a.g[i] =  dx%2;
					dx=dx/2;
				}

				for(int i=4; i<8;i++)
				{
					a.g[i] =  dy%2;
					dy=dy/2;
				}
				m_GeneList.push_back(a);
			}
		}
	}

	void	onMatingGene()
	{
		int count = m_GeneList.size();
		for(int i=0; i<count;i++)
		{
			for(int j=0; j<count;j++)
			{
				if(i!=j)
				{
					int pt = rand() % 8+1;
					int pt1 = 8 - pt;
					KNS_GENE a;
					memcpy(a.g,m_GeneList[i].g,sizeof(char)*pt);
					memcpy(&a.g[pt],&m_GeneList[j].g[pt1],sizeof(char)*pt1);					

					int r = rand()%10;
					if(r ==1)
					{
						int t = rand()%8;
						if(a.g[t]==0)
							a.g[t]=1;
						else
							a.g[t]=0;
					}

					m_GeneList.push_back(a);
				}
			}
		}
	}

	void	 onReleaseGene()
	{
		int count = m_GeneList.size()*0.7;

		for(int i=0;i<count;i++)
		{
			m_GeneList.erase(m_GeneList.end()-1);
		}
	}

	float			GetCheckGene(int id)
	{
		int x = m_GeneList[id].g[0]+m_GeneList[id].g[1]*2+m_GeneList[id].g[2]*4+m_GeneList[id].g[3]*8;
		int y = m_GeneList[id].g[4]+m_GeneList[id].g[5]*2+m_GeneList[id].g[6]*4+m_GeneList[id].g[7]*8;
		m_GeneList[id].x= x;
		m_GeneList[id].y= y;

		if(y<MAPSIZE&&x<MAPSIZE&&x>=0&&y>=0)
		{
			if(m_ppMap[x][y]==0)
			{				
				float tThisWinRate =GetCheckWinrate(this->m_nMyID,x,y);
				float tOtherWinRate =GetCheckWinrate(3 - this->m_nMyID,x,y);
				float g1 =1.0f-m_fg;
				float t = (tThisWinRate * m_fg) + (tOtherWinRate * g1);
				float p2 = 0.0f;
				if (tThisWinRate > 0.9f)
				{
					p2 = tThisWinRate;
					m_GeneList[id].re = tThisWinRate;
				}
				else if (tOtherWinRate > 0.9f)
				{
					p2 = tOtherWinRate;
					m_GeneList[id].re = tOtherWinRate;
				}
				else if (tOtherWinRate > 0.7f)
				{				
					p2 = tOtherWinRate;
					m_GeneList[id].re = tOtherWinRate;
				}
				else if(tThisWinRate > 0.7f)
				{
					p2 = tThisWinRate;
					m_GeneList[id].re = tThisWinRate;
				}
				else
				{
					p2 = t;
					m_GeneList[id].re = p2;
				}
								
				return p2;
			}
			m_GeneList[id].re=0;
			return 0;
		}
		else
		{
			m_GeneList[id].re=0;
			return 0;
		}
	}
	float			GetCheckWinrate(int mid, int x, int y)
	{
		int count[4]={0,};
		
		// --------------------------------- 좌측 우측 확인-------------------------------------------------
		bool left=false;
		bool right=false;
		

		for(int i=1;i<8;i++)
		{
			if(right==true&&left==true)
				break;

			int dx1 = x-i;
			int dx2 = x+i;

			if (m_ppMap[dx1][y] == 3 - mid || m_ppMap[dx2][y] == 3 - mid)
			{
				count[0]--;
				if (count[0] <= 0)
					count[0] = 0;
				break;
			}

			if(m_ppMap[dx1][y]==mid&&left==false&&dx1>=0&&dx1<MAPSIZE)
			{
					count[0]++;					
			}
			else
				left=true;
			if(m_ppMap[dx2][y]==mid&&right==false&&dx2>=0&&dx2<MAPSIZE)
			{				
					count[0]++;				
			}
			else
				right=true;
		}
		left = false;
		right = false;

		// --------------------------------- 위 아래 확인-------------------------------------------------

		for(int i=1;i<8;i++)
		{
			if(right==true&&left==true)
				break;

			int dy1 = y-i;
			int dy2 = y+i;

			if (m_ppMap[x][dy1] == 3 - mid || m_ppMap[x][dy2] == 3 - mid)
			{
				count[1]--;
				if (count[1] <= 0)
					count[1] = 0;
				break;
			}

			if(m_ppMap[x][dy1]==mid&&left==false&&dy1>=0&&dy1<MAPSIZE)
			{
				count[1]++;
			}
			else
				left=true;
			if(m_ppMap[x][dy2]==mid&&right==false&&dy2>=0&&dy2<MAPSIZE)
			{
				count[1]++;
			}
			else
				right=true;
		}

		left=false;
		right=false;

		// --------------------------------------- 대각선 확인-------------------------------------------------
		for(int i=1;i<8;i++)
		{
			if(right==true&&left==true)
				break;

			int dx1 = x+i;
			int dy1 = y-i;

			int dx2 = x-i;
			int dy2 = y+i;

			if (m_ppMap[dx1][dy1] == 3 - mid || m_ppMap[dx2][dy2] == 3 - mid)
			{
				count[2]--;
				if (count[2] <= 0)
					count[2] = 0;
				break;
			}

			if(m_ppMap[dx1][dy1]==mid&&left==false&&dx1>=0&&dx1<MAPSIZE&&dy1>=0&&dy1<MAPSIZE)
			{
				count[2]++;
			}
			else
				left=true;
			if(m_ppMap[dx2][dy2]==mid&&right==false&&dx2>=0&&dx2<MAPSIZE&&dy2>=0&&dy2<MAPSIZE)
			{
				count[2]++;
			}
			else
				right=true;
		}

		left=false;
		right=false;
		// --------------------------------------- 대각선 확인-------------------------------------------------

		for(int i=1;i<8;i++)
		{
			if(right==true&&left==true)
				break;

			int dx1 = x+i;
			int dy1 = y+i;

			int dx2 = x-i;
			int dy2 = y-i;

			if (m_ppMap[dx1][dy1] == 3 - mid || m_ppMap[dx2][dy2] == 3 - mid)
			{
				count[3]--;
				if (count[3] <= 0)
					count[3] = 0;
				break;
			}

			if(m_ppMap[dx1][dy1]==mid&&left==false&&dx1>=0&&dx1<MAPSIZE&&dy1>=0&&dy1<MAPSIZE)
			{
				count[3]++;
			}
			else
				left=true;
			if(m_ppMap[dx2][dy2]==mid&&right==false&&dx2>=0&&dx2<MAPSIZE&&dy2>=0&&dy2<MAPSIZE)
			{
				count[3]++;
			}
			else
				right=true;
		}
		left = false;
		right = false;


		int Max=0;
		for(int i=0; i<4;i++)
		{
			if(Max<=count[i])
				Max= count[i];
		}


		float p = (float)Max/(float)4.0f;
		return p;
	}
public:
	cKNS_GA(){}
	~cKNS_GA(){}

	void Init(int (*data)[MAPSIZE],int myid)
	{
		m_fg = 0.1f;
		
		srand(time(NULL));
		m_ppMap=data;
		m_GeneList.clear();
		m_nMyID =myid;
		m_nMaxgeneration=4;
		m_nInitGeneCount=8;
	}

	void* FrameMove(int x, int y)
	{
		m_vUserPos= Vector2f(x,y);
		onCreateGene();
		for(int n=0; n<m_nMaxgeneration;n++)
		{
			for(int i=0;i<m_GeneList.size();i++)
			{
				GetCheckGene(i);
			}
			sort(m_GeneList.begin(), m_GeneList.end(), KNS_comp);

			onReleaseGene();
			if(n!= (m_nMaxgeneration-1))
				onMatingGene();

		}
		Vector2f p = Vector2f(m_GeneList[0].x,m_GeneList[0].y);

		m_GeneList.clear();

		return &p;
	}

	void Release()
	{
		m_GeneList.clear();
	}

};

#endif