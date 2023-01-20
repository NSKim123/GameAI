#ifndef MAINSCENE_H__
#define MAINSCENE_H__

class CMainScene
{
private:
	int m_nHeroHp;
	int m_nEnemyHp;
	int m_nState;

private:

public:
	CMainScene()
	{

	}
	~CMainScene()
	{

	}

	void init()
	{
		printf("Init!!\n");
	}
	void Release()
	{
		printf("Release!!\n");
	}
	void FrameMove()
	{
		printf("FrameMove!!\n");
	}
	void SetHeroInfo(int hp)
	{
		printf("SetHeroInfo!!\n");
	}
	void SetEnemyInfo(int hp)
	{
		printf("SetEnemyInfo!!\n");
	}
	int GetState()
	{
		printf("GetState!!\n");
		return 0;
	}
};

#endif

