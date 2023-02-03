#ifndef GA_H
#define GA_H

struct GENE
{
	char g[8];
	float re; //�켺, ���� ��Ÿ���� ��ġ
	int x;
	int y;
};

class cGA
{
private:
	float m_fg;  //����
	vector<GENE> m_GeneList;
	int m_nInitGeneCount;
	int m_nMaxgeneration;
	int m_nMyID;//��? ������?
	int(*m_ppMap)[MAPSIZE];
	Vector2f m_vUserPos;

	
public:

private:
	void			onCreateGene();  
	void			onMatingGene();
	void			onReleaseGene();
	float			GetCheckGene(int id);
	float			GetCheckWinrate(int mid, int x, int y);
public:
	cGA();
	~cGA();

	void				Init(int (*data)[MAPSIZE],int myid);
	Vector2f				FrameMove(int x, int y);
	void				Release();

};

#endif