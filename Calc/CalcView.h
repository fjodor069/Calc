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
	SpreadSheetArea GetMouseLocation(CPoint ptMouse, Reference& rfCell);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnInitialUpdate();
private:
	CCalcDoc* m_pCalcDoc;
	bool m_bDoubleClick;
	Reference m_rfFirstCell;


};

#ifndef _DEBUG  // debug version in CalcView.cpp
inline CCalcDoc* CCalcView::GetDocument() const
   { return reinterpret_cast<CCalcDoc*>(m_pDocument); }
#endif

