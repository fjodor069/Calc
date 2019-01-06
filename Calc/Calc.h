// Calc.h : main header file for the Calc application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
//
// all other custom includes to be used in the project
// and forward declarations
class Cell;
class SyntaxTree;
class CellMatrix;
class Token;

#include "check.h"
#include "Color.h"
#include "Font.h"
#include "Caret.h"
#include "..\..\Utility\MyList.h"
#include "..\..\Utility\MySet.h"
#include "Reference.h"
#include "Token.h"
#include "SyntaxTree.h"
#include "Cell.h"
#include "CellMatrix.h"
#include "Scanner.h"
#include "Parser.h"
#include "TSetMatrix.h"


// CCalcApp:
// See Calc.cpp for the implementation of this class
//

class CCalcApp : public CWinApp
{
public:
	CCalcApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCalcApp theApp;