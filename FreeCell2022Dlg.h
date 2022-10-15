
// FreeCell2022Dlg.h : header file
//

#pragma once
#include "cell.h"

// CFreeCell2022Dlg dialog
class CFreeCell2022Dlg : public CDialogEx
{
// Construction
public:
	CFreeCell2022Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FREECELL2022_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CImage mCardImages[52];

	Cell* mCells[16];
	int mFirstClick; //remembers first clicked cell of a two click move. -1 means nothing clicked
	int mDeck[52];
	int mPreviousDeck[52];
	int mUndoStartCell;
	int mUndoEndCell;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void SwapDeckContents();
	void CreateSolvedDeck(int deck);
	void CreateRandomDeck();
	void PushmDeckToCells();
	void RestartCurrentGame();
	void UndoLastMove();
	void NewGame();
	void NewSolvedGame();
	void RestartPreviousGame();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMenuNewgame();
	afx_msg void OnMenuQuit();
	afx_msg void OnMenuRestartcurrentgame();
	afx_msg void OnMenuSwitchdeckpattern();
	afx_msg void OnMenuUndolastmove();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMenuRestartpreviousgame();
	afx_msg void OnMenuNewsolvedgame();
};
