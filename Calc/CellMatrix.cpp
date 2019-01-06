#include "stdafx.h"
//#include "check.h"
//#include "Cell.h"
//#include "Reference.h"
//#include "CellMatrix.h"

#include "Calc.h"


CellMatrix::CellMatrix(void)
{
}


//
// the copy constructor copies the cells one by one 
// and sets the matrix pointer for each cell
CellMatrix::CellMatrix(const CellMatrix& cellMatrix) 
{
	for (int iRow = 0; iRow < ROWS; ++iRow)
	{
		for (int iCol = 0; iCol < COLS; ++iCol)
		{
			m_buffer[iRow][iCol] = cellMatrix.m_buffer[iRow][iCol];
			m_buffer[iRow][iCol].SetCellMatrix(this);
		}
	}
}


// the assignment operator
CellMatrix CellMatrix::operator=(const CellMatrix & cellMatrix)
{
	return CellMatrix();
}

void CellMatrix::SetTargetSetMatrix(TSetMatrix * pTargetSetMatrix)
{
}

Cell* CellMatrix::Get(int iRow, int iCol) const
{
	check((iRow >= 0) && (iRow < ROWS));
	check((iCol >= 0) && (iCol < COLS));
	return (Cell*) &m_buffer[iRow][iCol];
}

Cell* CellMatrix::Get(Reference home) const
{
	return Get(home.GetRow(),home.GetCol());
}

void CellMatrix::Serialize(CArchive & ar)
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
