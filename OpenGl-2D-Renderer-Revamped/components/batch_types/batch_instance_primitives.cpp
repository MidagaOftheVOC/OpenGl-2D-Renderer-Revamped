#include "batch_instance_primitives.h"

//	TODO: finish this file and test the changes

uint16_t SpriteInformation::s_SheetSpriteBitmask = 0;

uint16_t SpriteInformation::s_SheetIndexBits = 5;
uint16_t SpriteInformation::s_SpriteIndexBits = 11;


void SpriteInformation::InitialiseMasks() {
	for (int i = 0; i < s_SheetIndexBits; i++) {
		s_SheetSpriteBitmask |= 1ui16 << (15 - i);
	}
}


SpriteInformation::SpriteInformation(
	unsigned int _sheetIndex,
	unsigned int _spriteIndex
) {

	uint16_t self = 0;

	self |= _sheetIndex << 16 - s_SheetIndexBits;
	self |= _spriteIndex;

	m_SheetSpriteIndexData = self;
}


uint16_t SpriteInformation::GetSheetIndex() const {
	return uint16_t(m_SheetSpriteIndexData & s_SheetSpriteBitmask) >> 16 - s_SheetIndexBits;
}


uint16_t SpriteInformation::GetSpriteIndex() const{
	return uint16_t(m_SheetSpriteIndexData & ~s_SheetSpriteBitmask);
}



