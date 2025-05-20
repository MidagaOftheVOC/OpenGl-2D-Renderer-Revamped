#include "text.h"


Text::Text(
	const std::u32string& _string
) :
	m_TextContent(_string)
{}


Text::Text(
	const Font* _font
) :
	m_Font(_font)
{}


Text::Text(
	const Font* _font,
	const std::u32string& _string
) :
	m_Font(_font),
	m_TextContent(_string)
{}


void Text::UpdateTextValue(
	std::u32string&& _stringToMove
) {
	m_TextContent = _stringToMove;
	UpdateGlyphOffsets();
}


void Text::UpdateTextValue(
	const std::u32string& _stringToCopy
) {
	m_TextContent = _stringToCopy;
	UpdateGlyphOffsets();
}


void Text::UpdateGlyphOffsets() {
	GetFont()->GetConsecutiveOffsetsFromFirstGlyph(m_TextContent, m_OffsetsFromFirstGlyph);
}
