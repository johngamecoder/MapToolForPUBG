#pragma once

namespace FONT
{
	enum FontType
	{
		NORMAL,
		QUEST
	};
}

class FontManager
{
public:
	static FontManager*	Get();
	static void		Delete();
private:
	static FontManager*	instance;

	map<FONT::FontType, LPD3DXFONT> m_mapFont;

	FontManager();
	~FontManager();
public:
	LPD3DXFONT GetFont(FONT::FontType e);
};

