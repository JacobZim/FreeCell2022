// WindowsCards.h and WindowsCards.cpp
// This helps you display cards on the device context, using cards.dll.
// For CS 3500, Fall 2010.

// IMPORTANT! For this dll to read correctly, you must change the calling convention of your project to __stdcall(/Gz):
//		Project->Properties->Configuration Properties->C/C++->Advanced->Calling Convention-> __stdcall(/Gz)
//		This must be done in both Debug and Release builds!
// Also, cards.dll must be made available in Windows Vista and Windows 7.
//		So get cards.dll from an XP machine in C:\Windows\System32\, and put it in the same directory where your source files are.

// When your application starts, call InitializeCards();
// When your application ends, call UninstallCards(); (such as in an OnClose() handler)
// Inbetween, call DrawCard();

#pragma once

bool InitializeCards();
bool DrawCard(HDC hdc, int left, int top, int index, bool selected = false); // index is 0 to 51, for the different cards.
bool DrawCardExt(HDC hdc, int left, int top, int width, int height, int index, bool selected);
bool UninstallCards();

//bool IsBlackCard(int index); //if suit is 0 or 3, return true

// The card width and height get set in InitializeCards:
extern int gCardWidth; // 71 pixels
extern int gCardHeight; // 96 pixels


// The card index works like this:
/* index	suit		rank
   -----	----		----
	0		clubs		ace
	1		diamonds	ace
	2		hearts		ace
	3		spades		ace

	4		clubs		two
	...

	48		clubs		king
	49		diamond		king
	50		hearts		king
	51		spaces		king

Given "suit" as 0-3, and "rank" as 0-12
	"index" = rank*4 + suit, as 0-51

Given the index,
	rank = index/4
	suit = index%4

*/
