#include "StdAfx.h"
#include "Calc.h"

Reference::Reference(void)
{
	m_iRow = 0;
	m_iCol = 0;
}

Reference::~Reference(void)
{
}

Reference::Reference(int iRow, int iCol)
{
	m_iRow = iRow;
	m_iCol = iCol;
}

Reference::Reference(const Reference& reference)
{
	m_iRow = reference.m_iRow;
	m_iCol = reference.m_iCol;
}

Reference& Reference::operator=(const Reference& reference)
{
	if (this != &reference)
	{
		m_iRow = reference.m_iRow;
		m_iCol = reference.m_iCol;	
	}
	return *this;
}


// returns true if ref1 == ref2
//
BOOL operator==(const Reference& rfLeft, const Reference& rfRight)
{
	return (rfLeft.m_iRow == rfRight.m_iRow ) &&
			(rfLeft.m_iCol == rfRight.m_iCol );
}

BOOL operator<(const Reference& rfLeft, const Reference &rfRight)
{
	return (rfLeft.m_iRow < rfRight.m_iRow) ||
		((rfLeft.m_iRow == rfRight.m_iRow ) &&
		(rfLeft.m_iCol < rfRight.m_iCol ));
}

CString Reference::ToString () const
{
	CString stBuffer;
	stBuffer.Format(TEXT("%c%d"), (TCHAR) (TEXT('a') + m_iCol), m_iRow + 1);
	return stBuffer;
}

void Reference::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_iRow;
		ar << m_iCol;
	}
	else
	{
		ar >> m_iRow;
		ar >> m_iCol;
	}
}