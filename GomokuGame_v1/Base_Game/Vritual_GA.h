#ifndef VIRTUAL_GA_H
#define VIRTUAL_GA_H
#define MAPSIZE 13

class cVirtual_GA
{
private:

public:
	cVirtual_GA() {}
	virtual	~cVirtual_GA() = 0;

	virtual void Init(int(*data)[MAPSIZE], int myid) = 0;
	virtual void* FrameMove(int x, int y) = 0;
	virtual void Release() = 0;
	

};

#endif