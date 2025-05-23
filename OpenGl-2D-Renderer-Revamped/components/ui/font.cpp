#include "font.h"



unsigned int Font::s_CommonFontSizeVBO = 0;


Font::Font(
	const SpriteSheet* _initialisedSheet,
	const std::string& _name
):
	m_SpriteSheet(_initialisedSheet),
	m_FontName(_name)
{}


void Font::Init(
	const std::u32string& _containedGlyphs,
	unsigned short* _glyphOffsets,
	int _glyphCount
) {
	DEBUG_ASSERT(_glyphCount >= 0 && _glyphCount <= MAXIMUM_GLYPHS_PER_FONT, "Strange glyph count for Font [%s].", GetName().c_str());
	DEBUG_ASSERT(!_containedGlyphs.empty(), "Empty glyph indentifier passed to Font [%s]", GetName().c_str());
	DEBUG_ASSERT(_glyphOffsets != nullptr, "Glyph offset pointer is null for Font [%s]", GetName().c_str());

	m_GlyphIdentifier = _containedGlyphs;
	m_GlyphCount = _glyphCount;

	memcpy(m_GlyphOffsetsFromAdjecentGlyph, _glyphOffsets, _glyphCount * sizeof(unsigned short));
}


void Font::GetConsecutiveOffsetsFromFirstGlyph(
	const std::u32string& _text,
	std::vector<unsigned short>& OUT_preparedGlyphOffsets
) const {
	size_t VectorSize = OUT_preparedGlyphOffsets.size();

	if (OUT_preparedGlyphOffsets.capacity() * 2 < _text.size()) {
		DEBUG_LOG("Unprepared std::vector passed to Font [%s].", GetName().c_str());
		OUT_preparedGlyphOffsets.resize(_text.size() * 2);
		VectorSize = _text.size() * 2;
	}

	int TotalOffset = 0;
	for (size_t i = 0; i < VectorSize; i += 2) {	// increment by 2
		
		OUT_preparedGlyphOffsets[i] = TotalOffset;
		unsigned short GlyphIndex = -1;

#ifdef DEBUG__CODE
		unsigned short d_Result = GetOffsetForGlyph(_text[i / 2], &GlyphIndex);
		DEBUG_ASSERT(d_Result != unsigned(-1), "Character [\\u%d] in Font [%s] not found.", (int)_text[i], GetName().c_str());
		
		TotalOffset += d_Result;
#else
		TotalOffset += GetOffsetForGlyph(_text[i], &GlyphIndex);
#endif

		OUT_preparedGlyphOffsets[i + 1] = GlyphIndex;
	}
}


unsigned short Font::GetOffsetForGlyph(
	char32_t _char,
	unsigned short* OUT_glyphIndexWithinSpriteSheet
) const {
	for (size_t i = 0; i < m_GlyphIdentifier.size(); i++) {
		if (_char == m_GlyphIdentifier[i]) 
		{
			if (OUT_glyphIndexWithinSpriteSheet) {
				*OUT_glyphIndexWithinSpriteSheet = static_cast<unsigned short>(i);
			}
			return m_GlyphOffsetsFromAdjecentGlyph[i];
		}
	}
	return -1;
}


void Font::InitialiseCommonFontSizeVBO(
	float _commonFontSize
) {
	
	bool IsVBO = glIsVertexArray(s_CommonFontSizeVBO) == GL_TRUE;

	if (IsVBO) {
		DEBUG_ASSERT(0, "Reinitialising common static VBO for Font class.")
		return;
	}

	float TempArray[] = {
		0.f,				0.f,
		0.f,				_commonFontSize,
		_commonFontSize,	_commonFontSize,
		_commonFontSize,	0.f
	};

	
	glGenBuffers(1, &s_CommonFontSizeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, s_CommonFontSizeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TempArray), TempArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


unsigned int Font::GetCommonFontSizeVBO() {
	return s_CommonFontSizeVBO;
}
