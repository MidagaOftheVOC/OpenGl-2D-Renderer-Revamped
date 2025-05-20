#pragma once

#include "font.h"



class Text {

	std::u32string m_TextContent;
	std::vector<unsigned short> m_OffsetsFromFirstGlyph;
	
	const Font* m_Font = nullptr;

private:


	
public:

	Text() {}


	Text(
		const std::u32string& _string
	);


	Text(
		const Font* _font
	);


	Text(
		const Font* _font,
		const std::u32string& _string
	);


	void UpdateTextValue(
		std::u32string&& _stringToMove
	);


	void UpdateTextValue(
		const std::u32string& _stringToCopy
	);

public:

	const std::u32string& GetTextString() const { return m_TextContent; }
	const Font* GetFont() const { return m_Font; }

private:

	void UpdateGlyphOffsets();

};

