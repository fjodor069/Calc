#pragma once
//
// utility class
// from Packt Publishing 
// 
//

namespace Utility
{

	class Color
	{
	public:
		Color();

		Color(int red, int green, int blue);
		Color(COLORREF crRedGreenBlue);
		Color(const Color& color);
		operator COLORREF() const;
		Color& operator=(const Color& color);
		bool operator==(const Color& color) const;
		bool operator!=(const Color& color) const;
		void Serialize(CArchive& ar);
		Color Inverse() const;
		COLORREF ColorRef() const { return m_crRedGreenBlue; }
		COLORREF& ColorRef() { return m_crRedGreenBlue; }

	private:
		COLORREF m_crRedGreenBlue;

	};

	extern const Color
		AliceBlue, AntiqueWhite, Aqua, Aquamarine,
		Azure, Beige, Bisque, Black, BlanchedAlmond,
		Blue, BlueViolet, Brown, Burlywood, CadetBlue,
		Chartreuse, Chocolate, Coral, CornflowerBlue,
		Cornsilk, Crimson, Cyan, DarkBlue, DarkCyan,
		DarkGoldenRod, DarkGray, DarkGreen, DarkKhaki,
		DarkMagenta, DarkOliveGreen, DarkOrange, DarkOrchid,
		DarkRed, DarkSalmon, DarkSeaGreen, DarkSlateBlue,
		DarkSlateGray, DarkTurquoise, DarkViolet, DeepPink,
		DeepSkyBlue, DimGray, DodgerBlue, FireBrick,
		FloralWhite, ForestGreen, Fuchsia, Gainsboro,
		GhostWhite, Gold, GoldenRod, Gray, Green, GreenYellow,
		HoneyDew, HotPink, IndianRed, Indigo, Ivory, Khaki,
		Lavender, LavenderBlush, Lawngreen, LemonChiffon,
		LightBlue, LightCoral, LightCyan, LightGoldenRodYellow,
		LightGreen, LightGray, LightPink, LightSalmon,
		LightSeaGreen, LightSkyBlue, LightSlateGray,
		LightSteelBlue, LightYellow, Lime, LimeGreen, Linen,
		Magenta, Maroon, MediumAquamarine, MediumBlue,
		MediumOrchid, MediumPurple, MediumSeaGreen,
		MediumSlateBlue, MediumSpringGreen, MediumTurquoise,
		MediumVioletRed, MidnightBlue, MintCream, MistyRose,
		Moccasin, NavajoWhite, Navy, Navyblue, OldLace, Olive,
		OliveDrab, Orange, OrangeRed, Orchid, PaleGoldenRod,
		PaleGreen, PaleTurquoise, PaleVioletRed, PapayaWhip,
		PeachPuff, Peru, Pink, Plum, PowderBlue, Purple,
		Red, RosyBrown, RoyalBlue, SaddleBrown, Salmon,
		SandyBrown, SeaGreen, SeaShell, Sienna, Silver, SkyBlue,
		SlateBlue, SlateGray, Snow, SpringGreen, SteelBlue,
		SystemColor, Tan, Teal, Thistle, Tomato, Turquoise,
		Violet, Wheat, White, WhiteSmoke, Yellow, YellowGreen;

}//end namespace