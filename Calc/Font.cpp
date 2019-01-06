#include "stdafx.h"
#include "Font.h"

namespace Utility
{

	Font::Font(void)
	{
		::memset(&m_logFont, 0, sizeof(m_logFont));
	}

	Font::~Font(void)
	{
	}

	//
	// there are two standard functions for copying strings
	// strcpy for char and wcscpy for wchar_t 
	// and there is wcscpy_s as type-save version
	//
	Font::Font(CString stName, int iSize)
	{
		::memset(&m_logFont, 0, sizeof(m_logFont));
		wcscpy_s(m_logFont.lfFaceName, stName);
		m_logFont.lfHeight = iSize;
	}

	Font::Font(const LOGFONT &logFont)
	{
		//logFont = logFont;
		//::memcpy(
		::memset(&m_logFont, 0, sizeof(m_logFont));
		::memcpy(&m_logFont, &logFont, sizeof(m_logFont));
	}

	//copy constructor
	Font::Font(const Font &font)
	{
		::memset(&m_logFont, 0, sizeof(m_logFont));
		::memcpy(&m_logFont, &font.m_logFont, sizeof(m_logFont));
	}
	//
	// the size of a font is normally in typographical points
	// however to calculate the screen size of text written in a certain font
	// we need to calculate it in 1/100 mm
	// to translate a point to 1/100 mm we multiply with 2540 and then divide by 72
	//
	Font Font::PointsToMeters() const
	{
		LOGFONT logFont = m_logFont;

		logFont.lfWidth = (int)((double)2540 * logFont.lfWidth / 72);
		logFont.lfHeight = (int)((double)2540 * logFont.lfHeight / 72);
		return Font(logFont);

	}


	//assignment operator
	Font & Font::operator=(const Font & font)
	{
		if (this != &font)
		{
			::memset(&m_logFont, 0, sizeof(m_logFont));
			::memcpy(&m_logFont, &font.m_logFont, sizeof(m_logFont));
		}
		return *this;
	}

	//
	// the standard fuction memcmp works similar to memset
	//
	BOOL Font::operator ==(const Font& font) const
	{
		return(::memcmp(&m_logFont, &font.m_logFont, sizeof(m_logFont) == 0));
	}

	BOOL Font::operator!=(const Font & font) const
	{
		return 0;
	}

	void Font::Serialize(CArchive &archive)
	{
		if (archive.IsStoring())
		{
			archive.Write(&m_logFont, sizeof(m_logFont));
		}
		if (archive.IsLoading())
		{
			archive.Read(&m_logFont, sizeof(m_logFont));
		}
	}

}