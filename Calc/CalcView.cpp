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
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CCalcView construction/destruction

CCalcView::CCalcView()
{
	//empty

}

CCalcView::~CCalcView()
{
	//empty
}

BOOL CCalcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCalcView drawing

void CCalcView::OnDraw(CDC* pDC)
{
	CCalcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->DPtoLP(&rcClient);
	CPen pen(PS_SOLID, 0, Utility::LightGray);
	CPen *pOldPen = pDC->SelectObject(&pen);
	CBrush grayBrush(Utility::LightGray);
	CBrush *pOldBrush = pDC->SelectObject(&grayBrush);
	int iTotalWidth = HEADER_WIDTH + COLS * COL_WIDTH;
	int iTotalHeight = HEADER_HEIGHT + ROWS * ROW_HEIGHT;
	// The area outside the spreadsheet.
	pDC->Rectangle(iTotalWidth, 0, rcClient.right,
		iTotalHeight);
	pDC->Rectangle(0, iTotalHeight,
		rcClient.right, rcClient.bottom);
	// The headers have white background color.
	CBrush whiteBrush(Utility::White);
	pDC->SelectObject(&whiteBrush);
	// Top left corner of the spreadsheet (the all button).
	pDC->Rectangle(0, 0, HEADER_WIDTH, HEADER_HEIGHT);
	// The row header of the spreadsheet.
	int xScrollPos = GetScrollPos(SB_HORZ);
	int yScrollPos = GetScrollPos(SB_VERT);
	int iStartRow = yScrollPos / ROW_HEIGHT;
	int iStartCol = xScrollPos / COL_WIDTH;

	for (int iRow = iStartRow; iRow < ROWS; ++iRow)
	{
		int yPos = iRow * ROW_HEIGHT;
		yPos += HEADER_HEIGHT - yScrollPos;
		//TRACE("iRow %d, yPos %d",iRow,yPos);
		CString stBuffer;
		stBuffer.Format(TEXT("%d"), iRow + 1);
		CRect rcHeader(0, yPos, HEADER_WIDTH, yPos + ROW_HEIGHT);
		pDC->Rectangle(&rcHeader);
		pDC->DrawText(stBuffer, &rcHeader, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	// The column header of the spreadsheet.
	for (int iCol = iStartCol; iCol < COLS; ++iCol)
	{
		int xPos = iCol * COL_WIDTH;
		xPos += HEADER_WIDTH - xScrollPos;
		//TRACE("iCol %d, xPos %d", iCol, xPos);
		CString stBuffer;
		stBuffer.Format(TEXT("%c"), (TCHAR)(TEXT('A') + iCol));
		CRect rcHeader(xPos, 0, xPos + COL_WIDTH, HEADER_HEIGHT);
		pDC->Rectangle(&rcHeader);
		pDC->DrawText(stBuffer, &rcHeader, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	CPoint ptScroll(xScrollPos, yScrollPos);
	CSize szHeader(HEADER_WIDTH, HEADER_HEIGHT);
	pDC->SetWindowOrg(ptScroll - szHeader);
	int iCellStatus = m_pCalcDoc->GetCalcStatus();
	CellMatrix* pCellMatrix = m_pCalcDoc->GetCellMatrix();
	Reference rfEdit = m_pCalcDoc->GetEdit();
	Reference rfFirstMark = m_pCalcDoc->GetFirstMark();
	Reference rfLastMark = m_pCalcDoc->GetLastMark();

	// The cell space.
	int iMinRow = min(rfFirstMark.GetRow(),	rfLastMark.GetRow());
	int iMaxRow = max(rfFirstMark.GetRow(),	rfLastMark.GetRow());
	int iMinCol = min(rfFirstMark.GetCol(),	rfLastMark.GetCol());
	int iMaxCol = max(rfFirstMark.GetCol(),	rfLastMark.GetCol());

	for (int iRow = iStartRow; iRow < ROWS; ++iRow)
	{
		for (int iCol = iStartCol; iCol < COLS; ++iCol)
		{
			BOOL bEdit = FALSE, bMark = FALSE;
			switch (iCellStatus)
			{
			case CS_EDIT:
				bEdit = (iRow == rfEdit.GetRow()) &&
					(iCol == rfEdit.GetCol());
				bMark = FALSE;
				break;
			case CS_MARK:
				bEdit = FALSE;
				bMark = (iRow >= iMinRow) && (iRow <= iMaxRow) &&
					(iCol >= iMinCol) && (iCol <= iMaxCol);
				break;
			}
			CPoint ptTopLeft(iCol * COL_WIDTH, iRow * ROW_HEIGHT);
			Cell* pCell = pCellMatrix->Get(iRow, iCol);
			pCell->Draw(ptTopLeft, bEdit, bMark, pDC);
		}
	}

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
	ptPoint.x += GetScrollPos(SB_HORZ) - HEADER_WIDTH;
	ptPoint.y += GetScrollPos(SB_VERT) - HEADER_HEIGHT;
}


void CCalcView::LogicalPointToSheetPoint(CRect& rcRect)
{
	LogicalPointToSheetPoint(rcRect.TopLeft());
	LogicalPointToSheetPoint(rcRect.BottomRight());
}


void CCalcView::SheetPointToLogicalPoint(CPoint& ptPoint)
{
	ptPoint.x += HEADER_WIDTH - GetScrollPos(SB_HORZ);
	ptPoint.y += HEADER_HEIGHT - GetScrollPos(SB_VERT);
}


void CCalcView::SheetPointToLogicalPoint(CRect& rcRect)
{
	SheetPointToLogicalPoint(rcRect.TopLeft());
	SheetPointToLogicalPoint(rcRect.BottomRight());
}

//
// this method is called on several occasions
// to make a given area visible and update the scroll bar positions
//
void CCalcView::MakeCellVisible(CRect rcArea)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);

	SCROLLINFO scrollInfo;
	GetScrollInfo(SB_HORZ, &scrollInfo);
	int xFirst = scrollInfo.nPos;
	int xPage = scrollInfo.nPage;
	int xLast = xFirst + xPage;

	//if a cell is to the left of the first visible cell
	//scroll the horizontal bar to the left
	if (rcArea.left < xFirst)
	{
		SetScrollPos(SB_HORZ, rcArea.left);
		CRect rcUpdate(HEADER_WIDTH, 0, TOTAL_WIDTH, TOTAL_HEIGHT);
		dc.LPtoDP(rcUpdate);
		InvalidateRect(rcUpdate);
		UpdateWindow();
		m_pCalcDoc->UpdateCaret();
	}

	//if the cell is to the right of the last visible cell
	//scroll the horizontal bar to the right

	if (rcArea.right > xLast)
	{
		int iDistance = rcArea.right - xLast;
		iDistance += COL_WIDTH - iDistance % COL_WIDTH;
		SetScrollPos(SB_HORZ, xFirst + iDistance);
		CRect rcUpdate(HEADER_WIDTH, 0, TOTAL_WIDTH,
			TOTAL_HEIGHT);
		dc.LPtoDP(rcUpdate);
		InvalidateRect(rcUpdate);
		UpdateWindow();
		m_pCalcDoc->UpdateCaret();
	}

	GetScrollInfo(SB_VERT, &scrollInfo);

	int yFirst = scrollInfo.nPos;
	int yPage = scrollInfo.nPage;
	int yLast = yFirst + yPage;


	//if the cell is above the top visible cell
	//scroll the vertical bar up
	if (rcArea.top < yFirst)
	{
		SetScrollPos(SB_VERT, rcArea.top);
		CRect rcUpdate(0, HEADER_HEIGHT, TOTAL_WIDTH,
			TOTAL_HEIGHT);
		dc.LPtoDP(rcUpdate);
		InvalidateRect(rcUpdate);
		UpdateWindow();
		m_pCalcDoc->UpdateCaret();
	}

	//if the cell is below the last visible cell
	//scroll the vertical bar down
	if (rcArea.bottom > yLast)
	{
		int iDistance = rcArea.bottom - yLast;
		iDistance += ROW_HEIGHT - iDistance % ROW_HEIGHT;
		SetScrollPos(SB_VERT, yFirst + iDistance);
		CRect rcUpdate(0, HEADER_HEIGHT, TOTAL_WIDTH,
			TOTAL_HEIGHT);
		dc.LPtoDP(rcUpdate);
		InvalidateRect(rcUpdate);
		UpdateWindow();
		m_pCalcDoc->UpdateCaret();
	}


}

//determine if a cell is visible 
BOOL CCalcView::IsCellVisible(int iRow, int iCol)
{
	SCROLLINFO scrollInfo;

	GetScrollInfo(SB_VERT, &scrollInfo, SIF_POS | SIF_PAGE);
	int iFirstVisibleRow = scrollInfo.nPos / ROW_HEIGHT;
	int iLastVisibleRow = iFirstVisibleRow + scrollInfo.nPage / ROW_HEIGHT;
	GetScrollInfo(SB_HORZ, &scrollInfo, SIF_POS | SIF_PAGE);
	int iFirstVisibleCol = scrollInfo.nPos / COL_WIDTH;
	int iLastVisibleCol = iFirstVisibleCol + scrollInfo.nPage / COL_WIDTH;
	return ((iRow >= iFirstVisibleRow) &&
		(iRow <= iLastVisibleRow) &&
		(iCol >= iFirstVisibleCol) &&
		(iCol <= iLastVisibleCol));
	   
}

//
// this method takes the position of a mouse click in device coordinates 
// and returns one of four areas of the spreadsheet/ client window : 
// top left corner, row header, column header, or cell area
//
SpreadSheetArea CCalcView::GetMouseLocation(CPoint ptMouse, Reference& rfCell)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&ptMouse);

	// Is the mouse if the top left header box?
	if ((ptMouse.x <= HEADER_WIDTH) &&
		(ptMouse.y <= HEADER_HEIGHT))
	{
		rfCell.SetRow(0);
		rfCell.SetCol(0);
		return MS_ALL;
	}
	// Or is it in the row header?
	else if (ptMouse.x <= HEADER_WIDTH)
	{
		LogicalPointToSheetPoint(ptMouse);
		rfCell.SetRow(min(ROWS - 1, ptMouse.y / ROW_HEIGHT));
		rfCell.SetCol(0);
		return MS_ROW;
	}
	// Or is it in the column header?
	else if (ptMouse.y <= HEADER_HEIGHT)
	{
		LogicalPointToSheetPoint(ptMouse);
		rfCell.SetRow(0);
		rfCell.SetCol(min(COLS - 1, ptMouse.x / COL_WIDTH));
		return MS_COL;
	}
	// If not, is has to be in the actual cell space.
	else
	{
		LogicalPointToSheetPoint(ptMouse);
		rfCell.SetRow(min(ROWS - 1, ptMouse.y / ROW_HEIGHT));
		rfCell.SetCol(min(COLS - 1, ptMouse.x / COL_WIDTH));
		return MS_SHEET;
	}

		
}



int CCalcView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//added special code
	m_pCalcDoc = (CCalcDoc*)m_pDocument;
	ASSERT_VALID(m_pCalcDoc);


	return 0;
}


void CCalcView::OnSize(UINT nType, int cx, int cy)
{
	//CView::OnSize(nType, cx, cy);

	// OnSize is called every time the user changes the size of the window
	//it sets the scroll bars to reflect the new size

	CClientDC dc(this);
	OnPrepareDC(&dc);
	CRect rcClient(0, 0, cx, cy);
	dc.DPtoLP(&rcClient);

	SCROLLINFO scrollInfo;
	scrollInfo.fMask = SIF_PAGE;
	scrollInfo.nPage = rcClient.right - HEADER_WIDTH;
	SetScrollInfo(SB_HORZ, &scrollInfo);

	scrollInfo.fMask = SIF_PAGE;
	scrollInfo.nPage = rcClient.bottom - HEADER_HEIGHT;
	SetScrollInfo(SB_VERT, &scrollInfo);
	m_pCalcDoc->UpdateCaret();
}


void CCalcView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// this method is called directly after a device context is created
	//it sets the coordinate mapping to isotropic (horizontal and vertical units are equal)
	//the logical units are 1/100 mm
	pDC->SetMapMode(MM_ISOTROPIC);
	CSize szWindow(100 * pDC->GetDeviceCaps(HORZSIZE), 100 * pDC->GetDeviceCaps(VERTSIZE));
	CSize szViewport(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	pDC->SetWindowExt(szWindow);
	pDC->SetViewportExt(szViewport);
}


void CCalcView::OnSetFocus(CWnd* pOldWnd)
{

	Utility::Caret* pCaret = m_pCalcDoc->GetCaret();
	pCaret->OnSetFocus(this);
}


void CCalcView::OnKillFocus(CWnd* pNewWnd)
{
	CView::OnKillFocus(pNewWnd);

	Utility::Caret* pCaret = m_pCalcDoc->GetCaret();
	pCaret->OnKillFocus();
}


void CCalcView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	//CView::OnVScroll(nSBCode, nPos, pScrollBar);


	SCROLLINFO scrollInfo;
	GetScrollInfo(SB_VERT, &scrollInfo);
	int yPos = scrollInfo.nPos;
	int iOldRow = yPos / ROW_HEIGHT;

	//increase or decrease the line and check if scroll position is not exceeded


	switch (nSBCode)
	{
	case SB_LINEUP:
		yPos = max(0, yPos - ROW_HEIGHT);
		break;
	case SB_LINEDOWN:
		yPos = min(yPos + ROW_HEIGHT, scrollInfo.nMax);
		break;
	case SB_PAGEUP:
		yPos = max(0, yPos - (int)scrollInfo.nPage);
		break;
	case SB_PAGEDOWN:
		yPos = min(yPos + (int)scrollInfo.nPage,
			scrollInfo.nMax);
		break;
	case SB_THUMBPOSITION:
		yPos = scrollInfo.nTrackPos;
		break;
	}
	int iNewRow = (int)((double)yPos / ROW_HEIGHT + 0.5);

	//if the top visible row is altered the client area needs repainting
	//create a dc to transform the header to device coordinates
	//invalidate the client area in device coordinates excluding the header
	//update the caret 
	if (iOldRow != iNewRow)
	{
		SetScrollPos(SB_VERT, iNewRow * ROW_HEIGHT);
		CRect rcClient;
		GetClientRect(&rcClient);
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CSize szHeader(HEADER_WIDTH, HEADER_HEIGHT);
		dc.LPtoDP(&szHeader);
		CRect rcUpdate(0, szHeader.cy, rcClient.right, rcClient.bottom);
		InvalidateRect(rcUpdate);
		m_pCalcDoc->UpdateCaret();
	}

}


void CCalcView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CCalcView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDoubleClick = FALSE;

	//determine which part of the spreadsheet is clicked
	SpreadSheetArea eArea = GetMouseLocation(point, m_rfFirstCell);

	switch (eArea)
	{
	case MS_ALL:
		m_pCalcDoc->UnmarkAndMark(0, 0, ROWS - 1, COLS - 1);
		break;
	case MS_ROW:
		m_pCalcDoc->UnmarkAndMark(m_rfFirstCell.GetRow(), 0,
			m_rfFirstCell.GetRow(), COLS - 1);
		break;
	case MS_COL:
		m_pCalcDoc->UnmarkAndMark(0, m_rfFirstCell.GetCol(),
			ROWS - 1, m_rfFirstCell.GetCol());
		break;
	case MS_SHEET:
		m_pCalcDoc->UnmarkAndMark(m_rfFirstCell.GetRow(),
			m_rfFirstCell.GetCol(),
			m_rfFirstCell.GetRow(),
			m_rfFirstCell.GetCol());
		break;
	};
	m_pCalcDoc->UpdateCaret();

}


void CCalcView::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bLeftButtonDown = (nFlags & MK_LBUTTON);
	if (bLeftButtonDown && !m_bDoubleClick)
	{
		Reference rcCurrCell;
		SpreadSheetArea eArea = GetMouseLocation(point, rcCurrCell);
		switch (eArea)
		{
		case MS_ALL:
			m_pCalcDoc->UnmarkAndMark(0, 0, ROWS - 1, COLS - 1);
			break;
		case MS_ROW:
			m_pCalcDoc->UnmarkAndMark(rcCurrCell.GetRow(), 0,
				rcCurrCell.GetRow(), COLS - 1);
			break;
		case MS_COL:
			m_pCalcDoc->UnmarkAndMark(0, rcCurrCell.GetCol(),
				ROWS - 1, rcCurrCell.GetCol());
			break;
		case MS_SHEET:
			m_pCalcDoc->UnmarkAndMark(m_rfFirstCell.GetRow(),
				m_rfFirstCell.GetCol(),
				rcCurrCell.GetRow(),
				rcCurrCell.GetCol());
			break;
		}
	}

	   	  
}


void CCalcView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	m_bDoubleClick = TRUE;
	Reference rfCell;
	SpreadSheetArea eArea = GetMouseLocation(point, rfCell);
	if (eArea == MS_SHEET)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.DPtoLP(&point);
		LogicalPointToSheetPoint(point);
		m_pCalcDoc->DoubleClick(rfCell, point, &dc);
	}


}

//
// onkeydown is called for every keypress event
// for specific keys below the document class is notified
// and the state of the shift key
//
void CCalcView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_UP:
	case VK_DOWN:
	case VK_HOME:
	case VK_END:
	case VK_RETURN:
	case VK_ESCAPE:
	case VK_TAB:
	case VK_INSERT:
	case VK_DELETE:
	case VK_BACK:
		CClientDC dc(this);
		OnPrepareDC(&dc);
		BOOL bShiftKeyDown = (::GetKeyState(VK_SHIFT) < 0);
		m_pCalcDoc->KeyDown(nChar, &dc, bShiftKeyDown);
		break;
	}
}

//
// if the keypress generates a printable character
// this is notified to the document class
//
void CCalcView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (isprint(nChar))
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		m_pCalcDoc->CharDown(nChar, &dc);
	}
}


void CCalcView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	// 
	if (lHint != NULL)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CRect rcUpdate = *(CRect*)lHint;
		SheetPointToLogicalPoint(rcUpdate);
		dc.LPtoDP(&rcUpdate);
		InvalidateRect(&rcUpdate);
		UpdateWindow();
	}


}


void CCalcView::OnInitialUpdate()
{
	//CView::OnInitialUpdate();

	//  Added specialized code here
	//this method is called when the view is first visible
	//it is slightly complicated as we have to take the row and column headers into account
	CClientDC dc(this);
	OnPrepareDC(&dc);

	CRect rcClient;
	GetClientRect(rcClient);
	dc.DPtoLP(&rcClient);

	//the width and height of the client area for the cells area 
	//is determined by subtracting the size of the row and column headers
	//(defined in cell.h)
	int iPageWidth = rcClient.right - HEADER_WIDTH;
	int iPageHeight = rcClient.bottom - HEADER_HEIGHT;

	//the size of the horizontal scroll bar is the size of the columns plus the rest
	//between the page and column width; this makes sure the cells fit into the client area
	SCROLLINFO scrollInfo;

	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nPos = 0;
	scrollInfo.nMin = 0;
	scrollInfo.nPage = rcClient.right - HEADER_WIDTH;
	scrollInfo.nMax = COLS * COL_WIDTH + iPageWidth % COL_WIDTH - 1;
	SetScrollInfo(SB_HORZ, &scrollInfo);


	//the vertical scroll bar
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nPos = 0;
	scrollInfo.nMin = 0;
	scrollInfo.nPage = rcClient.bottom - HEADER_HEIGHT;
	scrollInfo.nMax = ROWS * ROW_HEIGHT + iPageHeight % ROW_HEIGHT - 1;
	SetScrollInfo(SB_VERT, &scrollInfo);

	m_pCalcDoc->UpdateCaret();


}
