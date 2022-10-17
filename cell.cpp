#include "pch.h"
#include "cell.h"
#include "WindowsCards.h"

Cell::Cell(double left, double top, double right, double bottom)
	: mLeft(left), mTop(top), mRight(right), mBottom(bottom)
{
}

void Cell::Draw(CDC* dc, double WX, double WY, int PX, int PY, bool selected)  //add CImage mCardImages as a parameter as well as a bool for isScooby
{
	COLORREF greenColor(RGB(0, 190, 0));
	CBrush greenBrush;
	greenBrush.CreateSolidBrush(greenColor);
	dc->SelectObject(&greenBrush);
	
	dc->Rectangle(	(int)(mLeft *PX/WX), int((mTop)   *PY/WY),
					(int)(mRight*PX/WX), int((mBottom)*PY/WY) );

	//Draw cards inside this cell:
	double left = (mLeft * PX / WX);
	double right = (mRight * PX / WX);
	double top = (mTop * PX / WX);
	double bottom = (mBottom * PX / WX);
	double width = right - left;
	double height = width * gCardHeight / gCardWidth;
	if (!IsEmpty())
	{
		unsigned int size = mCards.size();
		DrawCardExt(*dc, (int)left, (int)(top), (int)width, (int)height, mCards[size - 1], selected);
	} //if isScooby, use StretchBlt() with the same parameters to draw scooby cards
}

bool Cell::CanRemoveCard() {
	return !IsEmpty();
}

void Cell::Push(int index) {
	mCards.push_back(index);
}
bool Cell::IsEmpty() {
	return mCards.size() == 0;
}
int Cell::Pop() {
	unsigned int size = mCards.size();
	int index = mCards[size - 1];
	mCards.pop_back();
	return index;
}
int Cell::Top() {
	unsigned int size = mCards.size();
	return mCards[size - 1];
}

bool Cell::Contains(CPoint point, double WX, double WY, int PX, int PY) {
	double left = (mLeft * PX / WX);
	double right = (mRight * PX / WX);
	double top = (mTop * PX / WX);
	double bottom = (mBottom * PX / WX);

	if (point.x >= left && point.x <= right && point.y >= top && point.y <= bottom) //might have to flip comparison signs for top and bottom
		return true;
	else return false;
}
void Cell::Empty() {
	//if (IsEmpty())
	//	return;
	unsigned int size = mCards.size();
	for (int i = 0; i < size; i++) {
		mCards.pop_back();
	}
	return;
}



//StartCell Functions

StartCell::StartCell(double left, double top, double right, double bottom)
	: Cell(left, top, right, bottom)
{

}

void StartCell::Draw(CDC* dc, double WX, double WY, int PX, int PY, bool selected)
{
	COLORREF greenColor(RGB(0, 190, 0));
	CBrush greenBrush;
	greenBrush.CreateSolidBrush(greenColor);
	dc->SelectObject(&greenBrush);

	/*dc->Rectangle((int)(mLeft * PX / WX), int((mTop)*PY / WY),
		(int)(mRight * PX / WX), int((mBottom)*PY / WY));*/

	//Draw cars inside this cell:
	double left = (mLeft * PX / WX);
	double right = (mRight * PX / WX);
	double top = (mTop * PX / WX);
	double bottom = (mBottom * PX / WX);
	double width = right - left;
	double height = width * gCardHeight / gCardWidth;

	dc->Rectangle((int)(left), int(top),   //simplified version of the commented code above
		(int)(right), int(bottom));

	if (!IsEmpty())
	{
		double topOffset = 0;
		for (unsigned int i = 0; i < mCards.size(); i++)
		{
			DrawCardExt(*dc, (int)left, (int)(top+topOffset), (int)width, (int)height, mCards[i], selected && (mCards[i] == Top())); //&& i==mCards.size()-1
			topOffset += height * .3;;
		}
	}
}
bool StartCell::CanReceiveCard(int index) {
	if (IsEmpty()) {
		return true;
	}
	int rank = GetCardRank(Top());
	bool isBlack = IsBlackCard(Top());
	int newRank = GetCardRank(index);
	bool newIsBlack = IsBlackCard(index);
	if (newRank == rank - 1) {
		if (newIsBlack != isBlack) {
			return true;
		}
	}
	return false;
}



//FreeCell Functions

FreeCell::FreeCell(double left, double top, double right, double bottom)
	: Cell(left, top, right, bottom)
{

}
bool FreeCell::CanReceiveCard(int index) {
	if (IsEmpty()) {
		return true;
	}
	return false;
}



//EndCell Functions 

EndCell::EndCell(double left, double top, double right, double bottom)
	: Cell(left, top, right, bottom)
{

}
void EndCell::Draw(CDC* dc, double WX, double WY, int PX, int PY, bool selected) 
{
	COLORREF greenColor(RGB(0, 190, 0));
	CBrush greenBrush;
	greenBrush.CreateSolidBrush(greenColor);
	dc->SelectObject(&greenBrush);

	dc->Rectangle((int)(mLeft * PX / WX), int((mTop)*PY / WY),
		(int)(mRight * PX / WX), int((mBottom)*PY / WY));

	//Draw cards inside this cell:
	double left = (mLeft * PX / WX);
	double right = (mRight * PX / WX);
	double top = (mTop * PX / WX);
	double bottom = (mBottom * PX / WX);
	double width = right - left;
	double height = width * gCardHeight / gCardWidth;
	//bool selected = false;
	if (!IsEmpty())
	{
		DrawCardExt(*dc, (int)left, (int)(top), (int)width, (int)height, mCards[mCards.size() - 1], selected);
	}
}
bool EndCell::CanRemoveCard() {
	return false;
}
bool EndCell::CanReceiveCard(int index) {
	int newRank = GetCardRank(index);
	bool newIsBlack = IsBlackCard(index);
	if (IsEmpty()) {
		if (newRank == 0)
			return true;
		else
			return false;
	}
	int rank = GetCardRank(Top());
	bool isBlack = IsBlackCard(Top());
	
	if (newRank == rank + 1) {
		if (newIsBlack == isBlack) {
			return true;
		}
	}
	return false;
}
