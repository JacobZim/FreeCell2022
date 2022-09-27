
// FreeCell2022Dlg.cpp : implementation file
//

// Day 1 (of 3 day week)
// Define class Cell. Init all 16 into good positions. 
// Draw them with a good aspect ratio.

// Day 2
// Load a dll that draws cards
// Draw all 52 cards onto the dc, for practice
// Instead load the cards into the start cells and have the cells draw their own cards
//		For now, draw all cards in a cell staggered
//		Use Cell methods Push, Pop, Top, and IsEmpty
// Randomize the cards
// Add a OnClose() method to do cleanup.

// More Core Tasks:
// Two clicks moves a card, always
// Derive 3 classes from Cell
// Draw FreeCells and EndCells as Not staggered.
// Add Cell methods CanRemoveCard, CanAddCard for 3 types of Cells.
// Two clicks moves a card, only if it is legal
// A second click on the same cell deselects it
// Draw something if the user wins. Test it. can test by initializing all card but 1 to be in correct place

// Nice Tasks:
// Add Menus
//		Quit
//		Restart New Game
//		Restart Current Game
//		Undo last move
//		Support one extra deck

// Dream Tasks:
//	Restart Previous Game
//	Undo and Redo multiple moves
//	Double click moves
//	Auto move the lowest card to an EndCell.
//	Keyboard shortcuts for some menu items.
//	Move Multiple cards at once, but only if it's possible
//	Support multiple extra decks
//	Support decks that need the icons
//		Make your own custom deck

// Along the way:
//	Fix flickering with double buffering
//	Choose a good background color
//	Fix ugly image drawing
//

#include "pch.h"
#include "framework.h"
#include "FreeCell2022.h"
#include "FreeCell2022Dlg.h"
#include "afxdialogex.h"
#include "WindowsCards.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double gWX = 0;
double gWY = 0;
double gPX = 0;
double gPY = 0;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFreeCell2022Dlg dialog



CFreeCell2022Dlg::CFreeCell2022Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FREECELL2022_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFreeCell2022Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFreeCell2022Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CFreeCell2022Dlg message handlers

BOOL CFreeCell2022Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	bool ok = InitializeCards();
	if (!ok)
	{
		MessageBox(L"InitializeCards failed.", L"Error", MB_ICONHAND);
		exit(1);
	}

	double CELL_WIDTH = 12;
	double CELL_HEIGHT = CELL_WIDTH * gCardHeight / gCardWidth;
	double START_CELL_HEIGHT = CELL_HEIGHT * 3;
	double MARGIN = (100.0 - CELL_WIDTH * 8) / 9;
	for (int i = 0; i < 4; i++)
	{
		double left = MARGIN + i * (CELL_WIDTH + MARGIN);
		double right = left + CELL_WIDTH;
		double top = MARGIN;
		double bottom = top + CELL_HEIGHT;
		mCells[i] = new FreeCell(left, top, right, bottom);
	}
	for (int i = 4; i < 8; i++)
	{
		double left = MARGIN + i * (CELL_WIDTH + MARGIN);
		double right = left + CELL_WIDTH;
		double top = MARGIN;
		double bottom = top + CELL_HEIGHT;
		mCells[i] = new EndCell(left, top, right, bottom);
	}
	for (int i = 8; i < 16; i++)
	{
		double left = MARGIN + (i - 8) * (CELL_WIDTH + MARGIN);
		double right = left + CELL_WIDTH;
		double top = MARGIN + CELL_HEIGHT + MARGIN;
		double bottom = top + START_CELL_HEIGHT;
		mCells[i] = new StartCell(left, top, right, bottom);
	}

	//Add cards to the start cells:
	mCells[8]->Push(0);

	// NOTE that world coordinates are 100 by 92
	gWX = 100;
	gWY = MARGIN * 3 + CELL_HEIGHT + START_CELL_HEIGHT;

	// Set the desired client size:
	double scale = 5.0;
	int clientWidth = (int)(gWX * scale); // Set clientWidth and clientHeight to your desired size
	int clientHeight = (int)(gWY * scale);

	// Set the initial window size, to get the desired client size:

	// Calculate the scaling_factor. 96 is the default Windows DPI, unless DPI scaling is enabled.
	// For example, if the user set their display Scale to 300%, then dpi will be 96*3,
	//		and the resulting scaling_factor below will be 3.0.
	// In this example, the dialog will be sized such that the final client rect 
	//		(GetClientRect(&rect) from On Paint)
	//		will be (0, 0, 3*clientWidth, 3*clientHeight)
	UINT dpi = GetDpiForWindow(*this);
	float scaling_factor = static_cast<float>(dpi) / 96;

	RECT scaled_size;
	scaled_size.left = 0;
	scaled_size.top = 0;
	scaled_size.right = static_cast<LONG>(clientWidth * scaling_factor);
	scaled_size.bottom = static_cast<LONG>(clientHeight * scaling_factor);

	AdjustWindowRectExForDpi(&scaled_size, WS_OVERLAPPEDWINDOW, false, 0, dpi);
	SetWindowPos(nullptr, 0, 0, scaled_size.right - scaled_size.left, scaled_size.bottom - scaled_size.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFreeCell2022Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFreeCell2022Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		CRect rect;
		GetClientRect(&rect);
		// set offsets to maintain aspect ratio:
		gWY = (float)rect.bottom / rect.right * gWX;

		for (int i = 0; i < 16; i++)
		{
			mCells[i]->Draw(&dc, gWX, gWY, gPX, gPY);
		}
		/*
		int left = 20;
		int top = 20;
		int index = 0;
		bool selected = false;
		for (index = 0; index < 52; index++)
		{
			DrawCardExt(dc, left, top, 400, 500, index, selected);
			left += 20;
		}
		*/
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFreeCell2022Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFreeCell2022Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	gPX = cx;
	gPY = cy;

	// TODO: Add your message handler code here
	Invalidate();
}


void CFreeCell2022Dlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UninstallCards();

	for (int i = 0; i < 16; i++)
		delete mCells[i];

	CDialogEx::OnClose();
}


void CFreeCell2022Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// Determine which cell <point> was inside
	int currentClick = -1;
	for (int i = 0; i < 16; i++)
		if (mCells[i]->Contains(point, gWX, gWY, gPX, gPY))
			currentClick = i;
	if (currentClick == -1)
		return; //no cell was clicked on
	
	if (mFirstClick == -1) // first click of a two click move
	{
		if (mCells[currentClick]->CanRemoveCard())
		{
			mFirstClick = currentClick;
		}
		
	}
	else // second click of two click move
	{
		// Is it legal to do the move:
		int index = mCells[mFirstClick]->Top();
		if (mCells[currentClick]->CanReceiveCard(index))
		{
			//move a card from cell mFirstClick to cell currentClick
			int index = mCells[mFirstClick]->Pop();
			mCells[currentClick]->Push(index);  // does not check for legal moves yet, be careful
		}
		mFirstClick = -1;
	}

	Invalidate();

	CDialogEx::OnLButtonUp(nFlags, point);
}
