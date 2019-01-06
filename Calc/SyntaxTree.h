#pragma once


enum SyntaxTreeIdentity {ST_EMPTY, ST_ADD, ST_SUB, ST_MUL, ST_DIV, ST_PARENTHESES, ST_REFERENCE, ST_VALUE };



class SyntaxTree
{
public:
	SyntaxTree();
	~SyntaxTree();

	SyntaxTree(const SyntaxTree& syntaxTree);
	SyntaxTree& operator=(const SyntaxTree& syntaxTree);
	void CopySyntaxTree(const SyntaxTree& syntaxTree);
	
	SyntaxTree(SyntaxTreeIdentity eTreeId, SyntaxTree *pLeftTree, SyntaxTree *pRightTree);
	
	SyntaxTree(double dValue);

	SyntaxTree(Reference& reference);
	double Evaluate(BOOL bRecursive, const CellMatrix* pCellMatrix) const;
	ReferenceSet GetSourceSet() const;
	void UpdateReference(int iRows, int iCols);
	CString ToString() const;
	void Serialize(CArchive& ar);

private:
	SyntaxTreeIdentity m_eTreeId;
	double m_dValue;
	SyntaxTree *m_pLeftTree, *m_pRightTree;
	Reference m_reference;

};
