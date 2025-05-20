#pragma once

#include <string>


#include "../../common/common.h"
#include "../../common/standard_quad.h"


constexpr unsigned int MAXIMUM_GLYPHS_PER_FONT = 256;

class Font {

	//	Holds the graphical aspect of the font, i.e. the
	//	images we'll be rendering
	const SpriteSheet* m_SpriteSheet = nullptr;

	int m_GlyphCount = 0;
	unsigned short m_GlyphOffsetsFromAdjecentGlyph[MAXIMUM_GLYPHS_PER_FONT] = { 0 };


	std::u32string m_GlyphIdentifier;

	
	std::string m_FontName;

public:

	Font() {}


	Font(
		const SpriteSheet* _initialisedSheet,
		const std::string& _name
	);


	void Init(
		const std::u32string& _containedGlyphs,
		unsigned short* _glyphOffsets,
		int _glyphCount
	);


	void GetConsecutiveOffsetsFromFirstGlyph(
		const std::u32string& _text,
		std::vector<unsigned short>& OUT_preparedGlyphOffsets
	) const;

private:

	unsigned short GetOffsetForGlyph(
		char32_t _char
	) const;

public:

	const std::string& GetName() const { return m_FontName; }
	unsigned short GetGlyphCount() const { return m_GlyphCount; }
	const unsigned short* GetGlyphOffsets() const { return m_GlyphOffsetsFromAdjecentGlyph; }
	const SpriteSheet* GetFontSheet() const { return m_SpriteSheet; }

};

