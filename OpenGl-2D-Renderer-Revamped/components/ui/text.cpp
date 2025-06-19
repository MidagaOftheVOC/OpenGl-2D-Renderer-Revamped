#include "text.h"


unsigned int Text::s_VAO = 0;


Text::Text() {
	GenerateBuffers();
}


Text::Text(
	const Font* _font,
	const std::u32string& _string,
	TextOptions _textOptions
) :
	m_Font(_font),
	m_TextOptions(_textOptions)
{
	GenerateBuffers();
	UpdateTextValue(_string);
}


void Text::UpdateTextValue(
	std::u32string&& _stringToMove
) {
	m_TextContent = _stringToMove;
	UpdateGlyphOffsets();
	UpdateVBO();
	CalculateWordWraps();
}


void Text::UpdateTextValue(
	const std::u32string& _stringToCopy
) {
	m_TextContent = _stringToCopy;
	UpdateGlyphOffsets();
	UpdateVBO();
	CalculateWordWraps();
}


void Text::UpdateGlyphOffsets() {
	DEBUG_ASSERT(GetFont() != nullptr, "Font pointer in Text object is null. Object address is [%p].", this);
	GetFont()->GetConsecutiveOffsetsFromFirstGlyph(m_TextContent, m_OffsetsFromFirstGlyph);
}


void Text::AppendCharacter(
	char32_t _char
) {
	int PreparedOffset = GetFont()->GetOffsetForGlyph(_char);

	DEBUG_ASSERT(PreparedOffset != -1, "Appended strange glyph not found in Font [%s].", m_Font->GetName().c_str());

	PreparedOffset += m_OffsetsFromFirstGlyph[m_TextContent.size() - 1];


	if (m_OffsetsFromFirstGlyph.size() == m_TextContent.size()) {
		m_OffsetsFromFirstGlyph.emplace_back(PreparedOffset);
		UpdateVBO();
	}
	else // this clause implies there are less characters stored than offsets, so we just replace it
	{
		m_OffsetsFromFirstGlyph[m_TextContent.size() - 1] = PreparedOffset;
	}

	m_TextContent += _char;
	CalculateWordWraps();
}


void Text::RemoveCharacter(
	char32_t _char
) {
	m_TextContent.pop_back();
	CalculateWordWraps();
}


void Text::UpdateVBO() {

	glBindBuffer(GL_ARRAY_BUFFER, GetGlyphDataVBO());	//  1 u16 / instance
	

	if ((m_TextContent.size() * 2) > m_VBOlastReportedSize) {
		//	text has more chars than can be stored in the VBO
		//	therefore we allocate
		size_t SizeOfDataToSend = m_TextContent.size() * sizeof(unsigned short) * 2;
		glBufferData(GL_ARRAY_BUFFER, SizeOfDataToSend, m_OffsetsFromFirstGlyph.data(), GL_STATIC_DRAW);
		m_VBOlastReportedSize = m_TextContent.size() * 2;
	}
	else {
		// text can be contained within the allocated buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(unsigned short) * 2) * m_TextContent.size(), m_OffsetsFromFirstGlyph.data());
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Text::InitialiseCommonVAO() {
	
	bool IsVertex = glIsVertexArray(s_VAO) == GL_TRUE;
	DEBUG_ASSERT( !IsVertex, "Reinitialising common static VAO for class Text.");

	if (IsVertex) {
		return;
	}
	

	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);

	//g_StandardQuad.BindVertexBufferAt(0);
	//	Add buffer with font sizes, 20px by 20px in the Font class.
	//	Then query for it here and bind it.
	
	glBindBuffer(GL_ARRAY_BUFFER, Font::GetCommonFontSizeVBO());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 2,
		(void*) 0
	);
	
	
	g_StandardQuad.BindTexUVbufferAt(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_StandardQuad.m_IndexBuffer);
	


	//	configuration without bound array
	//	2 gen attribs in this buffer
	//	attrib2 = 2B (offset = 0)
	//	attrib3 = 2B (offset = 2)
	//	stride = 4B

	glBindVertexBuffer(2, 0, 0, 2 * sizeof(unsigned short));
	glEnableVertexAttribArray(2);
	glVertexAttribIFormat(2, 1, GL_UNSIGNED_SHORT, 0);
	glVertexAttribBinding(2, 2);
	glVertexBindingDivisor(2, 1);

	glBindVertexBuffer(3, 0, 1* sizeof(unsigned short), 2 * sizeof(unsigned short));
	glEnableVertexAttribArray(3);
	glVertexAttribIFormat(3, 1, GL_UNSIGNED_SHORT, 0);
	glVertexAttribBinding(3, 3);
	glVertexBindingDivisor(3, 1);
	
	glBindVertexArray(0);
}


void Text::BindCommonVAO() {
	glBindVertexArray(s_VAO);
}


void Text::UnbindCommonVAO() {
	glBindVertexArray(0);
}


void Text::GenerateBuffers() {
	if (glIsBuffer(m_GlyphDataVBO) == GL_TRUE) {
		DEBUG_ASSERT(0, "Text object attempts regenerating its VBO. [%p]", this);
		return;
	}
	//	FIXME: this shouldn't work but it does for some reason
	//	Should be done with glCreateBuffers() instead. Oh well.
	glGenBuffers(1, &m_GlyphDataVBO);	
}


void Text::BindUniqueBuffers() const {
	glVertexArrayVertexBuffer( s_VAO, 2, GetGlyphDataVBO(), 0, sizeof(unsigned short) * 2);
	glVertexArrayVertexBuffer( s_VAO, 3, GetGlyphDataVBO(), sizeof(unsigned short) , sizeof(unsigned short) * 2);
}


void Text::SetWordWrapBound(
	float _rightBound
) {
	m_TextOptions.m_LineLength = _rightBound;
	CalculateWordWraps();
}


void Text::SetMaximumCharactersPerLine(
	int _charsPerLine
) {
	m_MaximumCharsPerLine = _charsPerLine;
}


void Text::CalculateWordWraps() {

	m_LineBreaks.clear();
	m_LineLengths.clear();
	if ( fEqual(m_TextOptions.m_LineLength, 0.f) ) {
		return;		// no bounds, render in a line
	}


	std::vector<int>& LineBreaks = m_LineBreaks;
	std::vector<float>& LineLengths = m_LineLengths;

	float TotalProccessedLinesLength = 0.f;
	int TotalLinesProcessed = 0;
	float CurrentLineLength = 0.f;

	int IndexOfSpace = -1;
	bool LineHasSpace = false;
	float SavedBufferOffsetTillAfterLastSpaceOfCurrentLine = 0.f;

	for (size_t i = 0; i < m_TextContent.size(); i++) {

		char32_t Character = m_TextContent[i];

		//float BufferOffsetForThisCharacter = GetTotalOffsetForCharAt(i);
		float CurrentCharWidth = GetFont()->GetOffsetForGlyph(Character);
		
		CurrentLineLength += CurrentCharWidth;

		if (Character == U' ') {
			LineHasSpace = true;
			IndexOfSpace = static_cast<int>(i);
			SavedBufferOffsetTillAfterLastSpaceOfCurrentLine = CurrentLineLength;	// this is for LineLengths
			continue;
		}

		if (CurrentLineLength > GetRightWordWrapBound()) {
			//	we exceed bound
			//	these line lengths need to be used by the buffer offsets


			if(!LineHasSpace)	// no space in line => word is too long
			{
				//	so we include last char width
				LineLengths.emplace_back(CurrentLineLength - CurrentCharWidth);
				//	we line break after the last character of this line
				LineBreaks.emplace_back(static_cast<int>(i));
				//m_TextContent.insert(m_TextContent.begin() + i, '\t');

				TotalProccessedLinesLength += CurrentLineLength;
				CurrentLineLength = 0.f;
				TotalLinesProcessed++;
				//	important, fist wrapped char needs to be redone on the new line
				i--;
			}
			else	//	space found, we wrap after it
			{
				LineLengths.emplace_back(SavedBufferOffsetTillAfterLastSpaceOfCurrentLine);

				LineBreaks.emplace_back(static_cast<int>(IndexOfSpace));

				CurrentLineLength = 0.f;
				SavedBufferOffsetTillAfterLastSpaceOfCurrentLine = 0.f;
				LineHasSpace = false;
				i = IndexOfSpace;
				IndexOfSpace = 0;
			}
		}
	}
}


unsigned short Text::GetTotalOffsetForCharAt(
	int _charIndex
) {
	DEBUG_ASSERT(_charIndex * 2 <= m_OffsetsFromFirstGlyph.size(),
		"Indexing out of Glyph offsets array in Text object; std::u32string address [%p].", &m_TextContent);
	return m_OffsetsFromFirstGlyph[_charIndex * 2];
}

