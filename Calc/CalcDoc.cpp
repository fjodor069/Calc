// CalcDoc.cpp : implementation of the CCalcDoc class
//
// this is a standard part of a MFC application

#include "stdafx.h"


#include "Calc.h"


#include "CalcDoc.h"
#include "CalcView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCalcDoc

IMPLEMENT_DYNCREATE(CCalcDoc, CDocument)

BEGIN_MESSAGE_MAP(CCalcDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CCalcDoc::OnUpdateEditPaste)
	ON_COMMAND(ID_ALIGNMENT_HORIZONTALLEFT, &CCalcDoc::OnAlignmentHorizontalleft)
	ON_COMMAND(ID_EDIT_COPY, &CCalcDoc::OnEditCopy)
END_MESSAGE_MAP()


// CCalcDoc construction/destruction

CCalcDoc::CCalcDoc()
		 : m_eCalcStatus(CS_MARK),
		   m_eKeyboardState(KM_INSERT),
		   m_rfMinCopy(-1, -1),
		   m_rfMaxCopy(-1, -1)
{
	// TODO: add one-time construction code here
	m_cellMatrix.SetTargetSetMatrix(&m_tSetMatrix);
	m_tSetMatrix.SetCellMatrix(&m_cellMatrix);

}

CCalcDoc::~CCalcDoc()
{
}

BOOL CCalcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CCalcDoc serialization

void CCalcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CCalcDoc diagnostics

#ifdef _DEBUG
void CCalcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCalcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCalcDoc commands

void CCalcDoc::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}




//
// the method RepaintEditArea, RepaintMarkedArea, RepaintSet
// all update one or more cells of the spreadsheet
//
void CCalcDoc::RepaintEditArea() 
{
	CPoint ptTopLeft(m_rfEdit.GetCol() * COL_WIDTH,
		             m_rfEdit.GetRow() * ROW_HEIGHT);
	CSize szEditCell(COL_WIDTH, ROW_HEIGHT);
	CRect rcEditCell(ptTopLeft, szEditCell);
	UpdateAllViews(NULL,(LPARAM) &rcEditCell);
}

void CCalcDoc::RepaintMarkedArea()
{
	int iMinMarkedRow = min(m_rfFirstMark.GetRow(), m_rfLastMark.GetRow());
	int iMaxMarkedRow = max(m_rfFirstMark.GetRow(), m_rfLastMark.GetRow());
	int iMinMarkedCol = min(m_rfFirstMark.GetCol(), m_rfLastMark.GetCol());
	int iMaxMarkedCol = max(m_rfFirstMark.GetCol(), m_rfLastMark.GetCol());

	CPoint ptTopLeft(iMinMarkedCol * COL_WIDTH, iMinMarkedRow * ROW_HEIGHT);
	CPoint ptBottomRight((iMaxMarkedCol + 1) * COL_WIDTH, (iMaxMarkedRow + 1) * ROW_HEIGHT);
	CRect rcMarkedBlock(ptTopLeft, ptBottomRight);
	UpdateAllViews(NULL,(LPARAM) &rcMarkedBlock);

}

//
// if the user modifies the value of a cell all its targets need to be notified
// re-evaluated and repainted. so the reference set must be repainted cell by cell
//
void CCalcDoc::RepaintSet(const ReferenceSet &repaintSet) 
{
	//iterate through the STL set (Reference objects)
	//for (auto& ref : repaintSet)
	for (POSITION position = repaintSet.GetHeadPosition();
		 position != NULL;
		repaintSet.GetNext(position))

	{
		Reference ref = repaintSet.GetAt(position);
		int iRow = ref.GetRow();
		int iCol = ref.GetCol();
		CPoint ptCell(iCol * COL_WIDTH, iRow * ROW_HEIGHT);
		CSize szCell(COL_WIDTH, COL_HEIGHT);
		CRect rcCell(ptCell, szCell);
		UpdateAllViews(NULL, (LPARAM) &rcCell);
	}

}

//
// this method is called when the user double clicks with the left mouse button
// this starts editing of the cell where the user clicked
//
void CCalcDoc::DoubleClick(Reference rfCell, CPoint ptMouse, CDC *pDC) 
{
	UnmarkAndMark(rfCell.GetRow(), rfCell.GetCol(), rfCell.GetRow(), rfCell.GetCol());
	m_eCalcStatus = CS_EDIT;
	m_rfEdit = rfCell;
	Cell* pEditCell = m_cellMatrix.Get(m_rfEdit.GetRow(),m_rfEdit.GetCol());
	pEditCell->GenerateInputText();
	CPoint ptTopLeft(m_rfEdit.GetCol() * COL_WIDTH, m_rfEdit.GetRow() * ROW_HEIGHT);
	m_iInputIndex = pEditCell->MouseToIndex(ptMouse - ptTopLeft);
	pEditCell->GenerateCaretArray(pDC);
	RepaintEditArea();
	UpdateCaret();

}

void CCalcDoc::MakeCellVisible(Reference rfCell) 
{
	MakeCellVisible(rfCell.GetRow(),rfCell.GetCol());
}

void CCalcDoc::MakeCellVisible(int iRow, int iCol) 
{
	CPoint ptTopLeft(iCol * COL_WIDTH, iRow * ROW_HEIGHT);
	CRect rcCell(ptTopLeft, CSize(COL_WIDTH, COL_HEIGHT));
	CCalcView* pCalcView = (CCalcView*) m_Caret.GetView();
	pCalcView->MakeCellVisible(rcCell);
}

void CCalcDoc::UpdateCaret() 
{
	switch(m_eCalcStatus)
	{
	case CS_MARK:
		m_Caret.HideCaret();
		break;
	case CS_EDIT:
		CCalcView* pCalcView = (CCalcView*) m_Caret.GetView();
		if (pCalcView->IsCellVisible(m_rfEdit.GetRow(),m_rfEdit.GetCol()))
		{
			Cell* pEditCell = m_cellMatrix.Get(m_rfEdit);
			CPoint ptTopLeft(m_rfEdit.GetCol() * COL_WIDTH, m_rfEdit.GetRow() * ROW_HEIGHT);
			CRect rcCaret = ptTopLeft + pEditCell->IndexToCaret(m_iInputIndex);
			if (m_eKeyboardState == KM_INSERT)
			{
				rcCaret.right = rcCaret.left + 1;
			}
			pCalcView->SheetPointToLogicalPoint(rcCaret);
			m_Caret.SetAndShowCaret(rcCaret);
		}
		else
		{
			m_Caret.HideCaret();
		}
		break;

	}
}

//
// this method is a central and rather complex method.
// Its purpose is to unmark the marked cells and to mark the new block given 
// without neccessary updating
//
void CCalcDoc::UnmarkAndMark(int iNewFirstMarkedRow, int iNewFirstMarkedCol, int iNewLastMarkedRow, int iNewLastMarkedCol) 
{
	int iOldMinMarkedRow = min(m_rfFirstMark.GetRow(), m_rfLastMark.GetRow());
	int iOldMaxMarkedRow = max(m_rfFirstMark.GetRow(), m_rfLastMark.GetRow());
	int iOldMinMarkedCol = min(m_rfFirstMark.GetCol(), m_rfLastMark.GetCol());
	int iOldMaxMarkedCol = max(m_rfFirstMark.GetCol(), m_rfLastMark.GetCol());

	int iNewMinMarkedRow = min(iNewFirstMarkedRow, iNewLastMarkedRow);
	int iNewMaxMarkedRow = max(iNewFirstMarkedRow, iNewLastMarkedRow);
	int iNewMinMarkedCol = min(iNewFirstMarkedCol, iNewLastMarkedCol);
	int iNewMaxMarkedCol = max(iNewFirstMarkedCol, iNewLastMarkedCol);

	m_rfFirstMark.SetRow(iNewFirstMarkedRow);
	m_rfLastMark.SetRow(iNewLastMarkedRow);
	m_rfFirstMark.SetCol(iNewFirstMarkedCol);
	m_rfLastMark.SetCol(iNewLastMarkedCol);

	switch (m_eCalcStatus)
	{
	case CS_EDIT:
		{
			Cell* pCell = m_cellMatrix.Get(m_rfEdit);
			m_eCalcStatus = CS_MARK;
			try
			{
				pCell->EndEdit(m_rfEdit);
				pCell->EvaluateValue(FALSE);
				ReferenceSet repaintSet = m_tSetMatrix.EvaluateTargets(m_rfEdit);
				RepaintSet(repaintSet);
				SetModifiedFlag();
			}
			catch (const CString stMessage)
			{
				AfxGetApp()->GetMainWnd()->MessageBox(stMessage, TEXT("Parse Error."));
				RepaintEditArea();
			}
			UpdateCaret();
		}
		break;
	//
	// if the application is in the mark state we need to unmark the cells not included  
	// in the new marked cell block
	//
	case CS_MARK:
		for (int iRow = iOldMinMarkedRow; iRow <= iOldMaxMarkedRow; ++iRow)
		{
			for (int iCol = iOldMinMarkedCol; iCol <= iOldMaxMarkedCol; ++iCol)
			{
				if ((iRow < iNewMinMarkedRow) ||
					(iRow > iNewMaxMarkedRow) ||
					(iCol < iNewMinMarkedCol) ||
					(iCol > iNewMaxMarkedCol))
				{
					CPoint ptTopLeft(iCol * COL_WIDTH, iRow * ROW_HEIGHT);
					CRect rcCell(ptTopLeft, CSize(COL_WIDTH,ROW_HEIGHT));
					UpdateAllViews(NULL,(LPARAM) &rcCell);
				}
			}
		}
		break;

	}//end switch
	
	//
	// finally we traverse the new marked cell block and repaint all cells
	// not already marked in the previous marked cell block
	//
	for (int iRow = iNewMinMarkedRow; iRow <= iNewMaxMarkedRow; ++iRow)
	{
		for (int iCol = iNewMinMarkedCol; iCol <= iNewMaxMarkedCol; ++iCol)
		{
			if ((iRow < iOldMinMarkedRow) ||
				(iRow > iOldMaxMarkedRow) ||
				(iCol < iOldMinMarkedCol) ||
				(iCol > iOldMaxMarkedCol))
			{
					CPoint ptTopLeft(iCol * COL_WIDTH, iRow * ROW_HEIGHT);
					CRect rcCell(ptTopLeft, CSize(COL_WIDTH,ROW_HEIGHT));
					UpdateAllViews(NULL,(LPARAM) &rcCell);
			}
		}
	}

}

//
//  this method is called when the user presses a special character
//  like shift, enter or insert
//
void CCalcDoc::KeyDown(UINT uChar, CDC *pDC, BOOL bShiftKeyDown) 
{
	switch (uChar)
	{
	case VK_LEFT:
		LeftArrowKey(bShiftKeyDown);
		break;
	//...
	case VK_INSERT:
		m_eKeyboardState = (m_eKeyboardState == KM_INSERT) ? KM_OVERWRITE : KM_INSERT;
		break;

	case VK_RETURN:
		{
		int iNewFirstMarkedRow = min(m_rfFirstMark.GetRow() + 1, ROWS - 1);
		UnmarkAndMark(iNewFirstMarkedRow, m_rfFirstMark.GetCol(), iNewFirstMarkedRow, m_rfFirstMark.GetCol());
		MakeCellVisible(iNewFirstMarkedRow, m_rfFirstMark.GetCol());
		}
		break;
	}
	UpdateCaret();
}

//
// the method CharDown is called when the user presses a regular key
// between ASCII 32 and 122 
//
void CCalcDoc::CharDown(UINT uChar, CDC *pDC) 
{
	if (m_eCalcStatus == CS_MARK)
	{
		UnmarkAndMark(m_rfFirstMark.GetRow(), m_rfFirstMark.GetCol(), m_rfFirstMark.GetRow(), m_rfFirstMark.GetCol());
		m_eCalcStatus = CS_EDIT;
		m_rfEdit = m_rfFirstMark;
		m_iInputIndex = 0;
		Cell* pCell = m_cellMatrix.Get(m_rfEdit);
		pCell->SetInputText(TEXT(""));
	}
	MakeCellVisible(m_rfEdit);
	Cell* pCell = m_cellMatrix.Get(m_rfEdit);
	pCell->CharDown(uChar, m_iInputIndex++, m_eKeyboardState);
	pCell->GenerateCaretArray(pDC);
	RepaintEditArea();
	UpdateCaret();

}

void CCalcDoc::LeftArrowKey(BOOL bShiftKeyDown)
{
}

void CCalcDoc::RightArrowKey(BOOL bShiftKeyDown)
{
}

void CCalcDoc::UpArrowKey(BOOL bShiftKeyDown)
{
}

void CCalcDoc::DownArrowKey(BOOL bShiftKeyDown)
{
}

void CCalcDoc::HomeKey(BOOL bShiftKeyDown)
{
}

void CCalcDoc::EndKey(BOOL bShiftKeyDown)
{
}

void CCalcDoc::DeleteKey(CDC * pDC)
{
}

void CCalcDoc::BackspaceKey(CDC * pDC)
{
}




void CCalcDoc::OnAlignmentHorizontalleft()
{
	// TODO: Add your command handler code here
}


void CCalcDoc::OnEditCopy()
{
	// TODO: Add your command handler code here
}
