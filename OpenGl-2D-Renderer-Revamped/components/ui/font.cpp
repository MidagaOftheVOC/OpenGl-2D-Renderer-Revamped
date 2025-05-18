#include "font.h"



Font::Font(
	const SpriteSheet* _initialisedSheet,
	const std::string& _name
):
	m_SpriteSheet(_initialisedSheet),
	m_FontName(_name)
{}

void Font::Init(
	const std::u32string& _containedGlyphs,
	int* _glyphOffsets,
	int _glyphCount
) {
	DEBUG_ASSERT(_glyphCount >= 0 && _glyphCount <= MAXIMUM_GLYPHS_PER_FONT, "Strange glyph count for Font [%s].", GetName().c_str());
	DEBUG_ASSERT(!_containedGlyphs.empty(), "Empty glyph indentifier passed to Font [%s]", GetName().c_str());
	DEBUG_ASSERT(_glyphOffsets != nullptr, "Glyph offset pointer is null for Font [%s]", GetName().c_str());

	m_GlyphIdentifier = _containedGlyphs;
	m_GlyphCount = _glyphCount;

	memcpy(m_GlyphOffsetsFromAdjecentGlyph, _glyphOffsets, _glyphCount * sizeof(int));
}


void Font::GetConsecutiveOffsetsFromFirstGlyph(
	const std::u32string& _text,
	std::vector<int>& OUT_preparedGlyphOffsets
) {
	if (OUT_preparedGlyphOffsets.capacity() < _text.size()) {
		DEBUG_LOG("Unprepared std::vector passed to Font [%s].", GetName().c_str());
		OUT_preparedGlyphOffsets.reserve(_text.size());
	}

	OUT_preparedGlyphOffsets[0] = 0;

	int TotalOffset = 0;
	size_t VectorSize = OUT_preparedGlyphOffsets.size();
	for (size_t i = 1; i < VectorSize; i++) {
		
#ifdef DEBUG__CODE
		int d_Result = GetOffsetForGlyph(_text[i]);
		DEBUG_ASSERT(d_Result != -1, "Character [\\u%d] in Font [%s] not found.", _text[i], GetName().c_str());
		
		TotalOffset += d_Result;
#else

		TotalOffset += GetOffsetForGlyph(_text[i]);

#endif
		OUT_preparedGlyphOffsets[i] = TotalOffset;
	}
}


int Font::GetOffsetForGlyph(
	char32_t _char
) {
	for (size_t i = 0; i < m_GlyphIdentifier.size(); i++) {
		if (_char == m_GlyphIdentifier[i]) return m_GlyphOffsetsFromAdjecentGlyph[i];
	}
	return -1;
}
