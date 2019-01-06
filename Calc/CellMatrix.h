#pragma once

//
// this class handles the whole spreadsheet
//

//forward declaration of class
class Cell;
class TSetMatrix;


const int ROWS = 10;
const int COLS = 5;



class CellMatrix
{
public:
	CellMatrix(void);
	
	
	CellMatrix(const CellMatrix& cellMatrix);
	CellMatrix operator=(const CellMatrix& cellMatrix);
	void SetTargetSetMatrix(TSetMatrix* pTargetSetMatrix);
	Cell* Get(int iRow, int iCol) const;
	Cell* Get(Reference home) const;
	void Serialize(CArchive& ar);
private:
	Cell m_buffer[ROWS][COLS];
};
