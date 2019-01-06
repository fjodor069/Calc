#include "stdafx.h"

//#include "check.h"
//#include "Reference.h"
//#include "Cell.h"
//#include "CellMatrix.h"
//
//
//
//#include "TSetMatrix.h"
#include "Calc.h"

TSetMatrix::TSetMatrix(void)
{
}

TSetMatrix::~TSetMatrix(void)
{
}

//copy constructor
TSetMatrix::TSetMatrix(const TSetMatrix& tSetMatrix)
{

}

//assignment operator
TSetMatrix& TSetMatrix::operator=(const TSetMatrix& tSetMatrix)
{
	//if (this == &tSetMatrix)
		return *this;

}

void TSetMatrix::SetCellMatrix(CellMatrix * pCellMatrix)
{
}

void TSetMatrix::Serialize(CArchive & ar)
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





ReferenceSet* TSetMatrix::Get(int iRow, int iCol) const
{
	check((iRow >= 0) && (iRow < ROWS));
	check((iCol >= 0) && (iCol < COLS));
	return (ReferenceSet*) &m_buffer[iRow][iCol];
}

ReferenceSet* TSetMatrix::Get(Reference home) const
{
	return Get(home.GetRow(), home.GetCol());
}

//
// when the user adds or alters a formula it is essential that 
// no cyclic reference is added. This method throws an exception
// when it finds a cycle. it performs a recursive backward search
void TSetMatrix::CheckCircular(Reference home, ReferenceSet sourceSet)
{
	for (POSITION position = sourceSet.GetHeadPosition();
		 position != NULL; 
		 sourceSet.GetNext(position))
	{
		Reference source = sourceSet.GetAt(position);

		if (source == home)
		{
			CString stMessage = TEXT("Circular Reference.");
			throw stMessage;
		}
		Cell* pCell = m_pCellMatrix->Get(source);
		ReferenceSet nextSourceSet = pCell->GetSourceSet();
		CheckCircular(home,nextSourceSet);
	}
}

//
// when a cell is modified, it is essential that the formulas
// having referencees to the cell are notified and re-evaluated
// this method performs a breadth-first search by following the target sets forward
//
ReferenceSet TSetMatrix::EvaluateTargets(Reference home)
{
	Cell* pHome = m_pCellMatrix->Get(home);
	pHome->EvaluateValue(FALSE);
	ReferenceSet resultSet;
	resultSet.Add(home);
	ReferenceSet* pTargetSet = Get(home);
	ReferenceSet updateSet = *pTargetSet;
	while (!updateSet.IsEmpty())
	{
		Reference target = updateSet.GetHead();
		resultSet.Add(target);
		updateSet.Remove(target);
		Cell* pTarget = m_pCellMatrix->Get(target);
		pTarget->EvaluateValue(FALSE);
		ReferenceSet* pNextTargetSet = Get(target);
		updateSet.AddAll(*pNextTargetSet);

	}
	return resultSet;
}

// the method AddTargets traverses the source set of the cell
// with the given reference in the cell matrix and for each source cell
// adds the given cell as a target in the target set of the source cell
void TSetMatrix::AddTargets(Reference home) 
{
	Cell* pCell = m_pCellMatrix->Get(home);
	ReferenceSet sourceSet = pCell->GetSourceSet();
	for (POSITION position = sourceSet.GetHeadPosition();
		 position != NULL;
		 sourceSet.GetNext(position))
	{
		Reference source = sourceSet.GetAt(position);
		ReferenceSet* pTargetSet = Get(source);
		pTargetSet->Add(home);
	}

}

//
// RemoveTargets traverses the source set of the cell with
// the given reference in the cell matrix and for each source cell
// removes the given cell as a target in the target set of the source cell
//
void TSetMatrix::RemoveTargets(Reference home) 
{
	Cell* pCell = m_pCellMatrix->Get(home);
	ReferenceSet sourceSet = pCell->GetSourceSet();

	/*for (auto& position(sourceSet.cbegin());
		 position != sourceSet.cend(); 
		 position++)*/
	for (POSITION position = sourceSet.GetHeadPosition();
	  	 position != NULL;
		 sourceSet.GetNext(position))


	{
		Reference source = sourceSet.GetAt(position);
		ReferenceSet* pTargetSet = Get(source);
		pTargetSet->Remove(home);
	}
}