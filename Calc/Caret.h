#pragma once

namespace Utility
{

	class Caret
	{
	public:
		Caret(void);
		~Caret(void);

		void SetAndShowCaret(const CRect rcCaret);
		void HideCaret();
		void OnSetFocus(CView* pView);
		void OnKillFocus();

		CView* GetView() const { return m_pFocusView; }

	private:
		BOOL m_bVisible;
		CView* m_pFocusView;
		CRect m_rcCaret;
	};

}
