#ifndef STATE_H
#define STATE_H


class cState 
{
private:

	int					m_nturnid;
	int					m_nMap[MAPSIZE][MAPSIZE];
	Sprite				m_Map;
	Sprite				m_dot[MAPSIZE][MAPSIZE];

	cGA*				m_pGA;

	int					m_nlposx;
	int					m_nlposy;

	bool				m_bEnd;
	int					m_nFramenum;
public:

private:
	void				onDrawLine(Vector2f p1 ,  Vector2f p2, Color4f c,float width);
	void				onDrawMap();

	bool				IsWin(int mid, int x, int y);
public:
	cState();
	~cState();

	void				Init();
	void				FrameMove();
	void				Release();
	void				Render();
			

};

#endif