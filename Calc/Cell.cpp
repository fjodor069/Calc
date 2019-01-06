#include "stdafx.h"

//all includes in one single header
#include "Calc.h"



using namespace Utility;


// constants and enums 
const int HEADER_WIDTH = 1000;
const int HEADER_HEIGHT = 500;
const int COL_WIDTH = 4000;
const int COL_HEIGHT = 1000;
const int ROW_HEIGHT = 1000;  
const int TOTAL_WIDTH = HEADER_WIDTH + COLS * COL_WIDTH;
const int TOTAL_HEIGHT = HEADER_HEIGHT + ROWS * ROW_HEIGHT;

static const int CELL_MARGIN = 100;

//the writable part of the cell is slightly smaller than the cell area
//the margin is subtracted 
const int CELL_WIDTH = COL_WIDTH - 2 * CELL_MARGIN;
const int CELL_HEIGHT = ROW_HEIGHT - 2 * CELL_MARGIN;

//default constructor
Cell::Cell()
	: m_eCellState(CELL_TEXT),
	m_stText(TEXT("")),
	m_dValue(0),
	m_syntaxTree(NULL),
	m_bHasValue(FALSE),
	m_stInput(TEXT("")),
	m_stOutput(TEXT("")),
	m_font(),
	m_textColor(Black),
	m_backgroundColor(White),
	m_eHorizontalAlignment(Alignment::HALIGN_JUSTIFIED),
	m_eVerticalAlignment(Alignment::VALIGN_CENTER),
		m_sourceSet(),
		m_pCellMatrix(NULL),
		m_pTargetSetMatrix(NULL)

{

	m_caretRectArray.Add(CRect(0,0,0,0));

	

}

Cell::~Cell()
{
}


//copy constructor
Cell::Cell(const Cell& cell)
{
	m_eCellState = cell.m_eCellState;
	m_eHorizontalAlignment = cell.m_eHorizontalAlignment; 
	m_eVerticalAlignment = cell.m_eVerticalAlignment;
	
	m_syntaxTree = cell.m_syntaxTree;
	//m_textColor = cell.m
	//	m_backgroundColor(WHITE),
	m_pCellMatrix = cell.m_pCellMatrix;
	m_pTargetSetMatrix = cell.m_pTargetSetMatrix;

}

//assignment operator
Cell& Cell::operator=(const Cell& cell)
{
	if (this != &cell)
	{
		m_eCellState = cell.m_eCellState;
		m_eHorizontalAlignment = cell.m_eHorizontalAlignment; 
		m_eVerticalAlignment = cell.m_eVerticalAlignment;
		m_syntaxTree = cell.m_syntaxTree;
		//m_textColor = cell.m
		//	m_backgroundColor(WHITE),
		m_pCellMatrix = cell.m_pCellMatrix;
		m_pTargetSetMatrix = cell.m_pTargetSetMatrix;		
	}
	return *this;
}


void Cell::CopyCell(const Cell& cell)
{
	m_eCellState = cell.m_eCellState;
	m_syntaxTree = cell.m_syntaxTree;
	m_sourceSet = cell.m_sourceSet;
	m_stText = cell.m_stText;
	m_dValue = cell.m_dValue;
	m_stInput = cell.m_stInput;
	m_stOutput = cell.m_stOutput;
	m_eHorizontalAlignment = cell.m_eHorizontalAlignment;
	m_eVerticalAlignment = cell.m_eVerticalAlignment;
	m_textColor = cell.m_textColor;
	m_backgroundColor = cell.m_backgroundColor;
	m_font = cell.m_font;
	m_caretRectArray.Copy(cell.m_caretRectArray );

}

void Cell::Clear(Reference home)
{
	if (m_eCellState == CELL_FORMULA)
	{
		m_pTargetSetMatrix->RemoveTargets(home);
	}
	m_eCellState = CELL_TEXT;
	m_stText = TEXT("");
	m_stOutput = TEXT("");


}

void Cell::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		//TODO: add storing code
		//ar << m_iRow;

	}
	else
	{
		//TODO: add loading code
	}

}

void Cell::CharDown(UINT uNewChar, int iInputIndex, KeyboardState eKeyBoardMode)
{
	if (iInputIndex == m_stInput.GetLength())
	{
		m_stInput.AppendChar ((TCHAR) uNewChar);
	}
	else
	{
		switch(eKeyBoardMode)
		{
		case KM_INSERT:
			m_stInput.Insert(iInputIndex, (TCHAR) uNewChar);
			break;

		case KM_OVERWRITE:
			m_stInput.SetAt(iInputIndex, (TCHAR) uNewChar);
			break;

		}
	}

}

//
// the method GenerateCaretArray is a central method to the application
// when the user adds or removes a character in a cell, the position of 
// the caret must be updated. only if the cell has the input focus
// (this method also in the draw application)
void Cell::GenerateCaretArray(CDC* pDC)
{
	CFont cFont;

	//create and initialise the font for the cell text
	//and convert from points to logical units (1/100 mm)
	//
	cFont.CreateFontIndirect(m_font.PointsToMeters());
	CFont* pPrevFont = pDC->SelectObject(&cFont);
	int iTextWidth, iTextHeight;
	if (!m_stInput.IsEmpty())
	{
		CSize szText = pDC->GetTextExtent(m_stInput);
		iTextWidth = szText.cx;
		iTextHeight = szText.cy;
	}
	else
	{
		TEXTMETRIC textMetric;
		pDC->GetTextMetrics(&textMetric);
		iTextWidth = 0;
		iTextHeight = textMetric.tmHeight;
	}

	

	//the beginning of the text position must be defined 
	//with the current cell alignment
	int xLeftPos = 0;			//start position in horizontal direction
	int iSpaceWidth = 0;		//width of a space in justified horizontal alignment
	int yTopPos = 0;			//start position in vertical direction

	switch(m_eHorizontalAlignment)
	{
	case DT_LEFT:
		xLeftPos = 0;
		break;
	case DT_CENTER:
		xLeftPos = (CELL_WIDTH - iTextWidth) / 2;
		break;
	case DT_RIGHT:
		xLeftPos = CELL_WIDTH - iTextWidth;
		break;
	case DT_JUSTIFIED:
		CString stInputNoSpaces = m_stInput;
		int iSpaceCount = stInputNoSpaces.Remove(TEXT(' '));
		if (iSpaceCount > 0)
		{
			xLeftPos = 0;
			CSize szInputNoSpaces = pDC->GetTextExtent(stInputNoSpaces);
			iSpaceWidth = (CELL_WIDTH - szInputNoSpaces.cx) / iSpaceCount;

		}
		else
		{
			xLeftPos = 0;
		}
		break;

	}

	switch(m_eVerticalAlignment)
	{
	case DT_TOP:
		yTopPos = 0;
		break;
	case DT_VCENTER:
		yTopPos = (CELL_HEIGHT - iTextHeight) / 2;
		break;
	case DT_BOTTOM:
		yTopPos = CELL_HEIGHT - iTextHeight;
		break;

	}

	int xPos = xLeftPos;
	int iLength = m_stInput.GetLength();
	m_caretRectArray.SetSize(iLength + 1);
	for (int iIndex = 0; iIndex < iLength; ++iIndex)
	{
		CString stChar = m_stInput.Mid(iIndex,1);
		int iCharWidth;
		if ((stChar == TEXT(" ")) && (m_eHorizontalAlignment == DT_JUSTIFIED))
		{
			iCharWidth = iSpaceWidth;
		}
		else
		{
			iCharWidth = pDC->GetTextExtent(stChar).cx;
		}
		CRect rcChar(xPos, yTopPos, xPos + iCharWidth, yTopPos + iTextHeight);
		m_caretRectArray[iIndex] = rcChar;
		xPos += iCharWidth;
	}

	//finally we add the size of a character of average size
	//because the user may put the caret marker to the right of the last character
	TEXTMETRIC textMetric;
	pDC->GetTextMetrics(&textMetric);
	int iAverageCharWidth = textMetric.tmAveCharWidth;
	CRect rcLastChar(xPos, yTopPos, xPos + iAverageCharWidth, yTopPos + iTextHeight);
	m_caretRectArray[iLength] = rcLastChar;
	pDC->SelectObject(pPrevFont);
	
}


//
// examines the text of the cell with the help of the caret array
// and finds the index of the matching character
//
int Cell::MouseToIndex(CPoint ptMouse)
{
	ptMouse -= CSize(CELL_MARGIN, CELL_MARGIN);
	int iLength = m_stInput.GetLength();
	if (ptMouse.x < m_caretRectArray[0].left)
	{
		return 0;
	}
	for (int iIndex = 0; iIndex < iLength; ++iIndex)
	{
		if ((ptMouse.x >= m_caretRectArray[iIndex].left) &&
			(ptMouse.x < m_caretRectArray[iIndex].right))
		{
			return iIndex;
		}
	}
	return iLength;
}

CRect Cell::IndexToCaret(int iIndex)
{
	return CRect();
}


// this method does the actual drawing on the device context
//
// drawing of the text is rather straightforward 
// with the function DrawText
//
void Cell::Draw(CPoint ptTopLeft, BOOL bEdit, BOOL bMarked, CDC* pDC)
{
	CRect rcCell(ptTopLeft, CSize(COL_WIDTH, ROW_HEIGHT));
	CRect rcMargin(rcCell.left + CELL_MARGIN,
				   rcCell.top + CELL_MARGIN,
				   rcCell.right - CELL_MARGIN,
				   rcCell.bottom - CELL_MARGIN);
	
	Color penColor = (bEdit || bMarked) ? m_textColor.Inverse() : m_textColor;
	Color brushColor = (bEdit || bMarked) ? m_backgroundColor.Inverse() : m_backgroundColor;
	//draw the cell rectangle
	CPen pen(PS_SOLID, 0, penColor);
	CPen* pPrevPen = pDC->SelectObject(&pen);
	CBrush brush(brushColor);
	CBrush* pPrevBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(rcCell);
	pDC->SelectObject(pPrevPen);
	pDC->SelectObject(pPrevBrush);
	//draw the text
	pDC->SetTextColor(penColor);
	pDC->SetBkColor(brushColor);
	CFont cFont;
	cFont.CreateFontIndirect(m_font.PointsToMeters());
	CFont* hPrevFont = pDC->SelectObject(&cFont);
	CString stDisplay = bEdit ? m_stInput : m_stOutput;

	if (m_eHorizontalAlignment == DT_JUSTIFIED)
	{
		CString stTemp = stDisplay;
		int iSpaceCount = stTemp.Replace(TEXT(' '), TEXT('.'));
		CSize szDisplay = pDC->GetTextExtent(stDisplay);
		pDC->SetTextJustification(rcMargin.Width() - szDisplay.cx, iSpaceCount);
		pDC->DrawText(stDisplay, &rcMargin, DT_SINGLELINE | m_eVerticalAlignment);
		pDC->SetTextJustification(0,0);
	}
	else
	{
		pDC->DrawText(stDisplay, &rcMargin, DT_SINGLELINE | m_eHorizontalAlignment | m_eVerticalAlignment);
	}
	pDC->SelectObject(hPrevFont);

}

Color Cell::GetColor(int iColorType) const
{
	return Color();
}

void Cell::SetColor(int iColorType, const Color & textColor)
{
}

Alignment Cell::GetAlignment(Direction eDirection) const
{
	return Alignment();
}

void Cell::SetAlignment(Direction eDirection, Alignment eAlignment)
{
}

//
// when the user starts editing a cell the contents are displayed as input text
//
void Cell::GenerateInputText()
{
	switch(m_eCellState)
	{
	case CELL_TEXT:
		m_stInput = m_stText;
		break;
	case CELL_VALUE:
		m_stInput.Format(TEXT("%f"), m_dValue);
		m_stInput.TrimRight(TEXT('0'));
		m_stInput.TrimRight(TEXT('.'));
		break;
	case CELL_FORMULA:
		m_stInput = TEXT("=") + m_syntaxTree.ToString();
		break;
	}
}

//
// this method is called when the user presses Enter or Tab or the mouse
// and ends the editing of the cell. 
// this starts interpreting the input text of the cell,
// and may set it to text, value or formula state
void Cell::EndEdit(Reference home)
{
	CString stTrimInput = m_stInput;
	stTrimInput.Trim();
	if ((!stTrimInput.IsEmpty()) && (stTrimInput[0] == TEXT('=')))
	{
		Parser parser;
		SyntaxTree newSyntaxTree = parser.Formula(stTrimInput.Mid(1));
		ReferenceSet newSourceSet = newSyntaxTree.GetSourceSet();
		m_pTargetSetMatrix->CheckCircular(home, newSourceSet);
		m_eCellState = CELL_FORMULA;
		m_pTargetSetMatrix->RemoveTargets(home);
		m_syntaxTree = newSyntaxTree;
		m_sourceSet = newSourceSet;
		m_pTargetSetMatrix->AddTargets(home);

	}
	else if  (IsNumeric(stTrimInput))
	{
		m_eCellState = CELL_VALUE;
		m_dValue = _tstof(stTrimInput);
		m_stOutput.Format(TEXT("%f"), m_dValue);
		m_stOutput.TrimRight(TEXT('0'));
		m_stOutput.TrimRight(TEXT('.'));
		m_pTargetSetMatrix->RemoveTargets(home);
		//m_sourceSet.clear(); 
		m_sourceSet.RemoveAll();

	}
	else
	{
		m_eCellState = CELL_TEXT;
		m_stText = m_stInput;
		m_stOutput = m_stText;
		m_pTargetSetMatrix->RemoveTargets(home);
		//m_sourceSet.clear();
		m_sourceSet.RemoveAll();
	}

}

BOOL Cell::IsNumeric(CString stText)
{
	return 0;
}

BOOL Cell::HasValue(BOOL bRecursive)
{
	switch(m_eCellState)
	{
	case CELL_TEXT:
		return FALSE;
	case CELL_VALUE:
		return TRUE;
	case CELL_FORMULA:
		if (bRecursive)
			EvaluateValue(TRUE);
		return m_bHasValue;
	}
	return FALSE;
}

void Cell::EvaluateValue(BOOL bRecursive)
{
	if (m_eCellState == CELL_FORMULA)
	{
		try
		{
			m_dValue = m_syntaxTree.Evaluate(bRecursive, m_pCellMatrix);
			m_bHasValue = true;
			m_stOutput.Format(TEXT("%f"), m_dValue);
			m_stOutput.TrimRight(TEXT('0'));
			m_stOutput.TrimRight(TEXT('.'));
		}
		catch (const CString& stMessage)
		{
			m_bHasValue = FALSE;
			m_stOutput = stMessage;
		}
	}

}

void Cell::UpdateSyntaxTree(int iRows, int iCols)
{
	if (m_eCellState == CELL_FORMULA)
	{
		m_syntaxTree.UpdateReference(iRows,iCols);
		m_sourceSet = m_syntaxTree.GetSourceSet();
	}
}