#include "text.h"

Text::Text() {}

Text::Text(
	const std::u32string& _string,
	TextOptions _textOptions
) :
	m_TextContent(_string),
	m_TextOptions(_textOptions)
{
	m_HasChanged = true;
}

void Text::UpdateTextValue(
	std::u32string&& _stringToMove
) {
	m_TextContent = std::move(_stringToMove);
	m_HasChanged = true;
}

void Text::UpdateTextValue(
	const std::u32string& _stringToCopy
) {
	m_TextContent = _stringToCopy;
	m_HasChanged = true;
}

void Text::AppendCharacter(
	char32_t _char
) {
	m_TextContent += _char;
	m_HasChanged = true;
}

void Text::RemoveLastCharacter() {
	if (m_TextContent.size() > 0) {
		m_TextContent.pop_back();
		m_HasChanged = true;
	}
}

void Text::SetLineLength(
	float length
) {
	m_TextOptions.lineLength = length;
	m_HasChanged = true;
}

const std::vector<FullSprite>& Text::GetTextGeometry() const {
	if (m_HasChanged) {
		CalculateGeometry();
	}
	return m_TextGeometry;
}

void Text::CalculateGeometry() const {

	m_TextGeometry.clear();

	if (m_TextOptions.scrollType == TextScroll::Oneline) {
		float CurrentLineLength = 0;
		//	We render the right-most text which can be fitted in this distance
		//	except the character which will be cut-off, since this isn't supported in the engine yet.
		float DisplayedLength = m_TextOptions.lineLength;


	}

	if (m_TextOptions.scrollType == TextScroll::Multiline) {

	}

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

void Text::SetScrollType(
	TextScroll scrollType
) {
	m_TextOptions.scrollType = scrollType;
	m_HasChanged = true;
}