#include "StdAfx.h"


//#include "Reference.h"
//#include "Cell.h"
//#include "CellMatrix.h"
//
//#include "SyntaxTree.h"

#include "Calc.h"

//default constructor
SyntaxTree::SyntaxTree()
	: m_eTreeId(ST_EMPTY),
	  m_dValue(0),
	 m_pLeftTree(NULL),
	m_pRightTree(NULL),
	m_reference()
{
}

SyntaxTree::SyntaxTree(SyntaxTreeIdentity eTreeId, SyntaxTree *pLeftTree, SyntaxTree *pRightTree)
	: m_eTreeId(eTreeId), 
	  m_pLeftTree(pLeftTree),
	  m_pRightTree(pRightTree)
{
	//empty
}


// destructor
SyntaxTree::~SyntaxTree()

{
	switch(m_eTreeId)
	{
	case ST_ADD:
	case ST_SUB:
	case ST_MUL:
	case ST_DIV:
		delete m_pLeftTree;
		delete m_pRightTree;
		break;
	case ST_PARENTHESES:
		delete m_pLeftTree;
		break;
	}
}

//copy constructor
SyntaxTree::SyntaxTree(const SyntaxTree& syntaxTree)
{
	m_eTreeId = syntaxTree.m_eTreeId;
	m_pLeftTree = syntaxTree.m_pLeftTree;
	m_pRightTree = syntaxTree.m_pRightTree;
	m_dValue = syntaxTree.m_dValue;
	m_reference = syntaxTree.m_reference;
}
	

//assignment operator
SyntaxTree& SyntaxTree::operator=(const SyntaxTree& syntaxTree)
{
	if (this != &syntaxTree)
	{
		m_eTreeId = syntaxTree.m_eTreeId;
		m_pLeftTree = syntaxTree.m_pLeftTree;
		m_pRightTree = syntaxTree.m_pRightTree;
		m_dValue = syntaxTree.m_dValue;
		m_reference = syntaxTree.m_reference;	
	}
	
	return *this;
}

void SyntaxTree::CopySyntaxTree(const SyntaxTree& syntaxTree)
{
	
}

SyntaxTree::SyntaxTree(double dValue)
{
	m_eTreeId = ST_VALUE;
	m_dValue = dValue;
}

SyntaxTree::SyntaxTree(Reference& reference)
{
	m_reference = reference;
}

double SyntaxTree::Evaluate (BOOL bRecursive, const CellMatrix* pCellMatrix) const
{
	double dLeftValue;
	double dRightValue;
	double dRetValue;

	switch(m_eTreeId)
	{
	case ST_ADD:
		dLeftValue = m_pLeftTree->Evaluate(bRecursive, pCellMatrix);
		dRightValue = m_pRightTree->Evaluate(bRecursive, pCellMatrix);
		dRetValue = dLeftValue + dRightValue;
		break;

	case ST_SUB:
		dLeftValue = m_pLeftTree->Evaluate(bRecursive, pCellMatrix);
		dRightValue = m_pRightTree->Evaluate(bRecursive, pCellMatrix);
		dRetValue = dLeftValue - dRightValue;
		break;

	case ST_MUL:
		dLeftValue = m_pLeftTree->Evaluate(bRecursive, pCellMatrix);
		dRightValue = m_pRightTree->Evaluate(bRecursive, pCellMatrix);
		dRetValue = dLeftValue * dRightValue;
		break;

	case ST_DIV:
		dLeftValue = m_pLeftTree->Evaluate(bRecursive, pCellMatrix);
		dRightValue = m_pRightTree->Evaluate(bRecursive, pCellMatrix);
		if (dRightValue != 0)
		  dRetValue = dLeftValue / dRightValue;
		else
		{
			CString stMessage = TEXT("#DIVISION_BY_ZERO");
			throw stMessage;
		}
		break;

	case ST_PARENTHESES:
		{
			int iRow = m_reference.GetRow();
			int iCol = m_reference.GetCol();
			Cell* pCell = pCellMatrix->Get(iRow, iCol);
			if (pCell->HasValue(bRecursive))
				return pCell->GetValue();
			else
			{
				CString stMessage = TEXT("#MISSING VALUE");
				throw stMessage;
			}
		}
		break;

	case ST_VALUE:
		return m_dValue;

	default:
		return 0;
	}
	return dRetValue;

}

ReferenceSet SyntaxTree::GetSourceSet() const
{
	switch(m_eTreeId)
	{
	case ST_ADD:
	case ST_SUB:
	case ST_MUL:
	case ST_DIV:
		{
			ReferenceSet leftSet = m_pLeftTree->GetSourceSet();
			ReferenceSet rightSet = m_pRightTree->GetSourceSet();
			return ReferenceSet::Union(leftSet, rightSet);
			
		}

	case ST_PARENTHESES:
		return m_pLeftTree->GetSourceSet();

	case ST_REFERENCE:
		{
			ReferenceSet resultSet;
			resultSet.Add(m_reference);
			return resultSet;
		}

	default:
		ReferenceSet emptySet;
		return emptySet;
	}
}

void SyntaxTree::UpdateReference(int iRows, int iCols)
{
	
	switch(m_eTreeId)
	{
	case ST_ADD:
	case ST_SUB:
	case ST_MUL:
	case ST_DIV:
			m_pLeftTree->UpdateReference(iRows, iCols);
			m_pRightTree->UpdateReference(iRows, iCols);
			break;

	case ST_PARENTHESES:
		m_pLeftTree->UpdateReference(iRows, iCols);

	case ST_REFERENCE:
			int iRow = m_reference.GetRow();
			int iCol = m_reference.GetCol();
			int iNewRow = iRow + iRows;
			int iNewCol = iCol + iCols;
			if ((iNewRow < 0 ) || (iNewRow >= ROWS) ||
				(iNewCol < 0 ) || (iNewCol >= COLS))
			{
				CString stMessage;
				stMessage.Format(TEXT("Invalid reference: \"%c%d\"."),
								 (TCHAR) (TEXT('a') + iNewCol),
								 iNewRow + 1);
				throw stMessage;
			}
			m_reference.SetRow(iNewRow);
			m_reference.SetCol(iNewCol);
		
		break;
	}
	
}

CString SyntaxTree::ToString() const
{
	CString stResult;

	CString stLeftTree = m_pLeftTree->ToString();
	CString stRightTree = m_pRightTree->ToString();

	switch(m_eTreeId)
	{
	case ST_ADD:
		
		stResult.Format(TEXT("%s+%s"), stLeftTree, stRightTree);
		break;

	case ST_SUB:
		stResult.Format(TEXT("%s-%s"), stLeftTree, stRightTree);
		break;

	case ST_MUL:
		stResult.Format(TEXT("%s*%s"), stLeftTree, stRightTree);
		break;

	case ST_DIV:
		stResult.Format(TEXT("%s/%s"), stLeftTree, stRightTree);
		break;

	case ST_REFERENCE:
		stResult = m_reference.ToString();
		break;

	case ST_VALUE:
		{
			stResult.Format(TEXT("%f"), m_dValue);
			stResult.TrimRight(TEXT('0'));
			stResult.TrimRight(TEXT('.'));
		}
		break;

	
	}

	return stResult;
}

void SyntaxTree::Serialize(CArchive & ar)
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

