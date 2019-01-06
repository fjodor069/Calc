#pragma once

//#include <set>


class Reference
{
public:
	Reference(void);
	~Reference(void);

	Reference(int iRow, int iCol);
	Reference(const Reference& reference);
	Reference& operator=(const Reference& reference);
	int GetRow() const {return m_iRow;}
	int GetCol() const {return m_iCol;}
	void SetRow(int iRow) {m_iRow = iRow;}
	void SetCol(int iCol) {m_iCol = iCol;}
	friend BOOL operator==(const Reference &ref1, const Reference &ref2);
	friend BOOL operator<(const Reference &ref1, const Reference &ref2);
	CString ToString() const;
	void Serialize(CArchive& ar);
private:
	int m_iRow, m_iCol;

};

//iterator type for document items
//typedef std::set<std::shared_ptr<Reference>> ReferenceSet;

typedef MySet<Reference> ReferenceSet;



