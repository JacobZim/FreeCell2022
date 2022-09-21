#include "pch.h"
#include "cell.h"
#include "WindowsCards.h"

Cell::Cell(double left, double top, double right, double bottom)
	: mLeft(left), mTop(top), mRight(right), mBottom(bottom)
{
}

void Cell::Draw(CDC* dc, double WX, double WY, int PX, int PY)
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
		double topOffset = 0;
		bool selected = false;
		for (unsigned int i = 0; i < mCards.size(); i++)
		{
			DrawCardExt(*dc, (int)left, (int)(top + topOffset), (int)width, (int)height, mCards[i], selected);
			topOffset += height * .3;
		}
	}
}

bool Cell::CanRemoveCard() {
	return true;
}
bool Cell::CanReceiveCard(int index) {   //means no basic implementation, each child needs own implementation
	return true;
}

void Cell::Push(int index) {
}
bool Cell::IsEmpty() {
	return mCards.size() == 0;
}
int Cell::Pop() {
	return 0;
}
int Cell::Top() {
	return 0;
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

StartCell::StartCell(double left, double top, double right, double bottom)
	: Cell(left, top, right, bottom)
{

}

void StartCell::Draw(CDC* dc, double WX, double WY, int PX, int PY)
{
	COLORREF greenColor(RGB(0, 190, 0));
	CBrush greenBrush;
	greenBrush.CreateSolidBrush(greenColor);
	dc->SelectObject(&greenBrush);

	dc->Rectangle((int)(mLeft * PX / WX), int((mTop)*PY / WY),
		(int)(mRight * PX / WX), int((mBottom)*PY / WY));

	//Draw cars inside this cell:
	double left = (mLeft * PX / WX);
	double right = (mRight * PX / WX);
	double top = (mTop * PX / WX);
	double bottom = (mBottom * PX / WX);
	double width = right - left;
	double height = width * gCardHeight / gCardWidth;
	if (!IsEmpty())
	{
		double topOffset = 0;
		bool selected = false;
		for (unsigned int i = 0; i < mCards.size(); i++)
		{
			DrawCardExt(*dc, (int)left, (int)(top), (int)width, (int)height, mCards[0], selected ); //had mCards[size - 1]
		}
	}
}
bool StartCell::CanReceiveCard(int index) {
	return true;
}

FreeCell::FreeCell(double left, double top, double right, double bottom)
	: Cell(left, top, right, bottom)
{

}
bool FreeCell::CanReceiveCard(int index) {
	return true;
}



EndCell::EndCell(double left, double top, double right, double bottom)
	: Cell(left, top, right, bottom)
{

}
void EndCell::Draw(CDC* dc, double WX, double WY, int PX, int PY) {
	return;
}
bool EndCell::CanRemoveCard() {
	return true;
}
bool EndCell::CanReceiveCard(int index) {
	return true;
}