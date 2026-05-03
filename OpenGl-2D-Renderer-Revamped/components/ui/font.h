#pragma once

#include <string>

#include "../../common/common.h"
#include "../../common/standard_quad.h"

inline constexpr unsigned int MAXIMUM_GLYPHS_PER_FONT = 256;
inline constexpr unsigned int MAXIMUM_LETTERS = 32;

struct GlyphSprite {
	SpriteInstance instance;
	unsigned short advance;
};

class Font {

	//	Holds the graphical aspect of the font, i.e. the
	//	images we'll be rendering
	const SpriteSheet* m_SpriteSheet = nullptr;

	bool m_IsLowercaseOnly = false;

	unsigned short m_GlyphAdvances[MAXIMUM_GLYPHS_PER_FONT] = { 0 };

	std::u32string m_GlyphIdentifier;

	std::string m_FontName;

public:

	Font() {}

	Font(
		const SpriteSheet* _initialisedSheet,
		const std::string& _name
	);

	void Init(
		const std::u32string& glyphs,
		unsigned short* glyphOffsets,
		int glyphCount,
		bool lowercaseOnly = false
	);

	unsigned short GetCharacterAdvance(
		char32_t character
	) const;

	GlyphSprite GetGlyph(
		char32_t ch
	) const;

	float GetGlyphAdvance(
		char32_t ch
	) const;

private:

	char32_t TransformToLowercase(
		char32_t ch
	) const;

	size_t GetGlyphIndex(
		char32_t ch
	) const;

public:

	const std::string& GetName() const { return m_FontName; }
	const SpriteSheet* GetFontSheet() const { return m_SpriteSheet; }

};

