// CalcView.h : interface of the CCalcView class
//


#pragma once

enum SpreadSheetArea {MS_ALL, MS_ROW, MS_COL, MS_SHEET};

class CCalcView : public CView
{
protected: // create from serialization only
	CCalcView();
	DECLARE_DYNCREATE(CCalcView)

// Attributes
public:
	CCalcDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCalcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void LogicalPointToSheetPoint(CPoint& ptPoint);
	void LogicalPointToSheetPoint(CRect& rcRect);
	void SheetPointToLogicalPoint(CPoint& ptPoint);
	void SheetPointToLogicalPoint(CRect& rcRect);
	void MakeCellVisible(CRect rcArea);
	BOOL IsCellVisible(int iRow, int iCol);
private:
	SpreadSheetArea GetMouseLocation(CPoint ptMouse, Reference& rcCell);
};

#ifndef _DEBUG  // debug version in CalcView.cpp
inline CCalcDoc* CCalcView::GetDocument() const
   { return reinterpret_cast<CCalcDoc*>(m_pDocument); }
#endif

