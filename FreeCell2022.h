
// FreeCell2022.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFreeCell2022App:
// See FreeCell2022.cpp for the implementation of this class
//

class CFreeCell2022App : public CWinApp
{
public:
	CFreeCell2022App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFreeCell2022App theApp;
