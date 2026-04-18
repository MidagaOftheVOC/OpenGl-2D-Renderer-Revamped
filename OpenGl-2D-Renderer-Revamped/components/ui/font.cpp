#include "font.h"

Font::Font(
	const SpriteSheet* _initialisedSheet,
	const std::string& _name
):
	m_SpriteSheet(_initialisedSheet),
	m_FontName(_name)
{}

void Font::Init(
    const std::u32string& upperCaseLetters,
    const std::u32string& lowerCaseLetters,
    const std::u32string& otherGlyphs,
    unsigned short* glyphOffsets,
    int glyphCount
)
{
    m_GlyphIdentifier.clear();
    m_GlyphIdentifier.reserve(
        upperCaseLetters.size() +
        lowerCaseLetters.size() +
        otherGlyphs.size() + 1
    );

    for (char32_t c : upperCaseLetters)
        m_GlyphIdentifier.push_back(c);

    for (char32_t c : lowerCaseLetters)
        m_GlyphIdentifier.push_back(c);

    m_GlyphIdentifier.push_back(U' ');

    for (char32_t c : otherGlyphs)
        m_GlyphIdentifier.push_back(c);

    memcpy(m_GlyphAdvances, glyphOffsets, glyphCount * sizeof(unsigned short));
}

unsigned short Font::GetCharacterAdvance(
    char32_t ch
) const {
    for (int i = 0; i < m_GlyphIdentifier.size(); i++)
    {
        if (m_GlyphIdentifier[i] == ch)
            return m_GlyphAdvances[i];
    }

    return 0;
}

size_t Font::GetGlyphIndex(
    char32_t ch
) const {
    for (int i = 0; i < m_GlyphIdentifier.size(); i++)
    {
        if (m_GlyphIdentifier[i] == ch)
            return i;
    }

    //  TODO: HACKFIX until cyrillic font fixes this
    return MAXIMUM_LETTERS * 2 - 2;
}

GlyphSprite Font::GetGlyph(
    char32_t ch
) const {

    size_t chIndex = GetGlyphIndex(ch);
    GlyphSprite result;
    result.instance = m_SpriteSheet->GetSpriteInstanceByIndex(chIndex, 0);
    result.advance = m_GlyphAdvances[chIndex];

    return result;
}