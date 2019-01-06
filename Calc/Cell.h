#pragma once

//
// this class handles a single cell
//
//constants defined in Cell.cpp
// are used in the View class
//
extern const int HEADER_WIDTH;
extern const int HEADER_HEIGHT;
extern const int COL_WIDTH; 
extern const int COL_HEIGHT;
extern const int ROW_HEIGHT;
extern const int TOTAL_WIDTH;
extern const int TOTAL_HEIGHT;


enum CellState {CELL_TEXT, CELL_VALUE, CELL_FORMULA};
typedef CArray<CRect> RectArray;
enum Alignment {HALIGN_LEFT = DT_LEFT, 
				HALIGN_RIGHT = DT_RIGHT,
			    HALIGN_JUSTIFIED = DT_LEFT + DT_CENTER + DT_RIGHT,
				VALIGN_TOP = DT_TOP,
				VALIGN_CENTER = DT_VCENTER,
				VALIGN_BOTTOM = DT_BOTTOM};

#define DT_JUSTIFIED  DT_LEFT + DT_CENTER + DT_RIGHT

enum KeyboardState {KM_INSERT, KM_OVERWRITE};
enum Direction {HORIZONTAL, VERTICAL};
enum ColorStatus {TEXT, BACKGROUND};

//forward declarations
class CellMatrix;
class TSetMatrix;
class SyntaxTree;


class Cell
{
public:
	Cell();
	~Cell();

	Cell(const Cell& cell);
	Cell& operator=(const Cell& cell);
	void CopyCell(const Cell& cell);
	void SetCellMatrix(CellMatrix* pCellMatrix) {m_pCellMatrix = pCellMatrix;}
	void SetTargetSetMatrix(TSetMatrix* pTargetSetMatrix) {m_pTargetSetMatrix = pTargetSetMatrix;}
	BOOL IsEmpty() const {return (m_eCellState == CELL_TEXT) && m_stText.IsEmpty();}
	void Clear(Reference home);
	void Serialize(CArchive& ar);
	void CharDown(UINT cChar, int iEditIndex, KeyboardState eKeyBoardMode);
	void GenerateCaretArray(CDC* pDC);
	CString GetInputText() {return m_stInput;}
	void SetInputText(CString stInput) {m_stInput = stInput;}
	int MouseToIndex(CPoint ptMouse);
	CRect IndexToCaret(int iIndex);
	void Draw(CPoint ptTopLeft, BOOL bEdit, BOOL bMarked, CDC* pDC);
	
	Utility::Font GetFont() const {return m_font;}
	void SetFont(const Utility::Font& font) {m_font = font;}
	Utility::Color GetColor(int iColorType) const;
	void SetColor(int iColorType, const Utility::Color& textColor);
	Alignment GetAlignment(Direction eDirection) const;
	void SetAlignment(Direction eDirection, Alignment eAlignment);
	void GenerateInputText();
	void EndEdit(Reference home);
	BOOL IsNumeric(CString stText);
	BOOL HasValue(BOOL bRecursive);
	double GetValue() const {return m_dValue; }
	void EvaluateValue(BOOL bRecursive);
	void UpdateSyntaxTree(int iAddRows, int iAddCols);
	ReferenceSet GetSourceSet() const {return m_sourceSet;}
private:
	CellState m_eCellState;
	CString m_stText;
	double m_dValue;
	

	BOOL m_bHasValue;
	RectArray m_caretRectArray;
	CString m_stInput, m_stOutput;
	Utility::Font m_font;
	Utility::Color m_textColor, m_backgroundColor;
	Alignment m_eHorizontalAlignment, m_eVerticalAlignment;
	ReferenceSet m_sourceSet;
	CellMatrix* m_pCellMatrix;
	TSetMatrix* m_pTargetSetMatrix;
	SyntaxTree m_syntaxTree;

};
