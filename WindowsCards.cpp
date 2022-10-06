#include "pch.h"
#include "WindowsCards.h"

// these are the functions that are exported by cards.dll. what we have here
// are function pointers to individual functions within the cards.dll module.
// when cards.dll is loaded, the module is queried for the address of each of the
// known exported functions. the returned function pointer values are then assigned
// to the corresponding variable below
typedef BOOL (*CARDINITFUNCPTR)(int* cardWidth, int* cardHeight);
typedef BOOL (*CARDDRAWFUNCPTR)(HDC hdc, int x, int y, int rankAndSuit, int mode, DWORD rgbBgnd); // rankAndSuit 0-51. mode 0(faceUp) 1(faceDown). rgbBgnd -1(highlighted) 0(regular)
typedef BOOL (*CARDDRAWEXTFUNCPTR)(HDC hdc, int x, int y, int dx, int dy, int rankAndSuit, int mode, DWORD rgbBgnd);
typedef BOOL (*CARDANIMATEFUNCPTR)(HDC hdc, int rankAndSuit, int x, int y, int ispr);
typedef VOID (*CARDTERMFUNCPTR)();

CARDINITFUNCPTR		CardInit;		// coincides with cdtInit
CARDDRAWFUNCPTR		CardDraw;		// coincides with cdtDraw
CARDDRAWEXTFUNCPTR	CardDrawExt;	// coincides with cdtDrawExt
CARDANIMATEFUNCPTR	CardAnimate;	// coincides with cdtAnimate
CARDTERMFUNCPTR		CardTerm;		// coincides with cdtTerm

int gCardWidth=0;
int gCardHeight=0;
bool gReady = false;
bool InitializeCards() {

	HINSTANCE hComponent = NULL;
	if((hComponent = ::LoadLibrary(L"cards.dll")) == NULL)
		return false;	// unable to load cards.dll

	// get the address of each of the exported functions from cards.dll
	CardInit = (CARDINITFUNCPTR)::GetProcAddress(hComponent,"cdtInit");
	if(CardInit == NULL)
		return false;	// couldn't locate function ctdInit(...).

	CardDraw = (CARDDRAWFUNCPTR)::GetProcAddress(hComponent,"cdtDraw");
	if(CardDraw == NULL)
		return false;	// couldn't locate function cdtDraw(...).

	CardDrawExt = (CARDDRAWEXTFUNCPTR)::GetProcAddress(hComponent,"cdtDrawExt");
	if(CardDrawExt == NULL)
		return false;	// couldn't locate function cdtDrawExt(...).

	CardAnimate = (CARDANIMATEFUNCPTR)::GetProcAddress(hComponent,"cdtAnimate");
	if(CardAnimate == NULL)
		return false;	// couldn't locate function cdtAnimate(...).

	CardTerm = (CARDTERMFUNCPTR)::GetProcAddress(hComponent,"cdtTerm");
	if(CardTerm == NULL)
		return false;	// couldn't locate function cdtTerm().

	// call the cards.dll initialization function.
	if(!CardInit(&gCardWidth,&gCardHeight))
		return false;

	gReady = true;
	return gReady;
} // end function Init ()

bool UninstallCards()
{
	if(!gReady)
		return false;

	CardTerm();
	gReady = false;
	return true;
}
/*
bool IsBlackCard(int index)
{
	if( (index%4) == )
} */


bool DrawCard(HDC hdc, int left, int top, int index, bool selected)
{
	if(!gReady)
		return false;

	int mode= selected ? 2 : 0; // 0 for faceUp, 2 for highlighted.

	int ok = CardDraw(hdc,				// handle to device context to display card on
					left,			// x position of upper-left corner of card
					top,			// y position of upper-left corner of card
					index,
					mode,
					0xFFFFFF);

	return (ok==1);
}
bool DrawCardExt(HDC hdc, int left, int top, int width, int height, int index, bool selected)
{
	if (!gReady)
		return false;

	int mode = selected ? 2 : 0; // 0 for faceUp, 2 for highlighted.

	int ok = CardDrawExt(hdc,				// handle to device context to display card on
		left,			// x position of upper-left corner of card
		top,			// y position of upper-left corner of card
		width,
		height,
		index,
		mode,
		0xFFFFFF);

	return (ok == 1);
}


bool IsBlackCard(int index) //if suit is 0 or 3, return true
{
	int suite = index % 4;
	if ((suite == 0) || (suite == 3)) {
		return true;
	}
	return false;
}
int GetCardRank(int index) {
	return index / 4;
}