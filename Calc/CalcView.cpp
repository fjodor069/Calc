// CalcView.cpp : implementation of the CCalcView class
//
//
// this is a standard part of a MFC application

#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "Calc.h"
#endif


#include "CalcDoc.h"
#include "CalcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCalcView

IMPLEMENT_DYNCREATE(CCalcView, CView)

BEGIN_MESSAGE_MAP(CCalcView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCalcView construction/destruction

CCalcView::CCalcView()
{
	// TODO: add construction code here

}

CCalcView::~CCalcView()
{
}

BOOL CCalcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCalcView drawing

void CCalcView::OnDraw(CDC* /*pDC*/)
{
	CCalcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CCalcView printing

BOOL CCalcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCalcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCalcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CCalcView diagnostics

#ifdef _DEBUG
void CCalcView::AssertValid() const
{
	CView::AssertValid();
}

void CCalcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCalcDoc* CCalcView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalcDoc)));
	return (CCalcDoc*)m_pDocument;
}
#endif //_DEBUG


// CCalcView message handlers


void CCalcView::LogicalPointToSheetPoint(CPoint& ptPoint)
{
}


void CCalcView::LogicalPointToSheetPoint(CRect& rcRect)
{
}


void CCalcView::SheetPointToLogicalPoint(CPoint& ptPoint)
{
}


void CCalcView::SheetPointToLogicalPoint(CRect& rcRect)
{
}


void CCalcView::MakeCellVisible(CRect rcArea)
{
}


BOOL CCalcView::IsCellVisible(int iRow, int iCol)
{
	return 0;
}


SpreadSheetArea CCalcView::GetMouseLocation(CPoint ptMouse, Reference& rcCell)
{
	return SpreadSheetArea();
}
