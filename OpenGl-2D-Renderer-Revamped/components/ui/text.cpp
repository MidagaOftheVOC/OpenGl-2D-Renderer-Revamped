#include "text.h"

Text::Text() {}

Text::Text(
	const std::u32string& _string,
	TextOptions _textOptions
) :
	m_TextContent(_string),
	m_TextOptions(_textOptions)
{
	CalculateGeometry();
	m_HasChanged = true;
}

void Text::UpdateTextValue(
	std::u32string&& _stringToMove
) {
	m_TextContent = std::move(_stringToMove);
	CalculateGeometry();
	m_HasChanged = true;
}

void Text::UpdateTextValue(
	const std::u32string& _stringToCopy
) {
	m_TextContent = _stringToCopy;
	CalculateGeometry();
	m_HasChanged = true;
}

//	TODO: mark for optimisation
//	Technically, appending one character shouldn't have much influence outside the closest space to the end and its new position
void Text::AppendCharacter(
	char32_t _char
) {
	m_TextContent += _char;
	CalculateGeometry();
	m_HasChanged = true;
}

void Text::RemoveLastCharacter() {
	if (m_TextContent.size() > 0) {
		m_TextContent.pop_back();
		CalculateGeometry();
		m_HasChanged = true;
	}
}

void Text::SetLineLength(
	float length
) {
	m_TextOptions.lineLength = length;
	CalculateGeometry();
	m_HasChanged = true;
}

const std::vector<FullSprite>& Text::GetTextGeometry() const {
	return m_TextGeometry;
}

void Text::CalculateGeometry() {

	m_TextGeometry.clear();

	int CurrentLineNumber = 0;
	float CurrentLineLength = 0;

	for (size_t index = 0; index < m_TextContent.size(); index++) {
		GlyphSprite glyph = GetFont()->GetGlyph(m_TextContent[index]);

		FullSprite self;
		self.instance = glyph.instance;
		self.position.x = CurrentLineLength;
		self.position.y = CurrentLineNumber * GetLineHeight();

		CurrentLineLength += glyph.advance;
		m_TextGeometry.emplace_back(self);
	}

	m_HasChanged = false;
}