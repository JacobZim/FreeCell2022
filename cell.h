#pragma once
#include <vector>

class Cell
{
public:
	Cell(double left, double top, double right, double bottom);
	virtual void Draw(CDC* dc, double WX, double WY, int PX, int PY, bool selected);
	virtual bool CanRemoveCard();
	virtual bool CanReceiveCard(int index)=0; //means no basic implementation, each child needs own implementation
	void Push(int index);
	bool IsEmpty();
	int Pop();
	int Top();
	bool Contains(CPoint point, double WX, double WY, int PX, int PY);
	

protected:
	double mLeft, mTop, mRight, mBottom;
	std::vector<int> mCards;
};

class StartCell : public Cell
{
public: 
	StartCell(double left, double top, double right, double bottom);
	void Draw(CDC* dc, double WX, double WY, int PX, int PY, bool selected); //double check this ?
	virtual bool CanReceiveCard(int index);
};

class FreeCell : public Cell
{
public:
	FreeCell(double left, double top, double right, double bottom);
	virtual bool CanReceiveCard(int index); //if empty return true, else false
};

class EndCell : public Cell
{
public:
	EndCell(double left, double top, double right, double bottom);
	virtual void Draw(CDC* dc, double WX, double WY, int PX, int PY, bool selected);
	virtual bool CanRemoveCard();
	virtual bool CanReceiveCard(int index); //if empty, should be ace (rank = 0), else, same suit and rank +1
};