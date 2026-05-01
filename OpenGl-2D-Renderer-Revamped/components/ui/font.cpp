#include "font.h"

Font::Font(
	const SpriteSheet* _initialisedSheet,
	const std::string& _name
):
	m_SpriteSheet(_initialisedSheet),
	m_FontName(_name)
{}

void Font::Init(
    const std::u32string& glyphs,
    unsigned short* glyphOffsets,
    int glyphCount,
    bool lowercaseOnly
)
{
    m_IsLowercaseOnly = lowercaseOnly;
    m_GlyphIdentifier.clear();
    m_GlyphIdentifier.reserve(
        glyphs.size()
    );

    for (char32_t c : glyphs)
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

    if (m_IsLowercaseOnly) {
        ch = TransformToLowercase(ch);
    }

    size_t chIndex = GetGlyphIndex(ch);
    GlyphSprite result;
    //  Sheet index is queried in the DrawText function, once per Text draw call
    result.instance = m_SpriteSheet->GetSpriteInstanceByIndex(chIndex, 0);  
    result.advance = m_GlyphAdvances[chIndex];

    return result;
}

char32_t Font::TransformToLowercase(char32_t ch) const
{
    switch (ch) {
    case U'Ѝ': return U'ѝ';
    case U'Ё': return U'ё';
    }

    if (ch >= U'А' && ch <= U'Я')
        return ch + 32;

    return ch;
}