#pragma once

#include "../../common/common.h"

//	Instance-related primitive structs for sets of data

struct SpriteInformation {

	SpriteInformation() {}

	
	SpriteInformation(
		unsigned int _sheetIndex,
		unsigned int _spriteIndex
	);


	uint16_t GetSheetIndex() const;
	uint16_t GetSpriteIndex() const;

private:

	uint16_t m_SheetSpriteIndexData = 0;

public:	//	Bitmasks

	//	Can only be called once.
	static void InitialiseMasks();

private:

	//	0 - masks the sprite index
	//	1 - masks the sheet index
	static uint16_t s_SheetSpriteBitmask;

	static uint16_t s_SheetIndexBits;
	static uint16_t s_SpriteIndexBits;

};

struct xyPair {
	float x = -1, y = -1;
};

struct SpriteInstance {
	SpriteInformation SpriteInfo;	//	ushort
	xyPair dimensions;				//	float *2

	bool IsNull() { return fEqual(dimensions.x, -1) || fEqual(dimensions.y, -1); }
};

struct FullSprite {
	SpriteInstance instance;//	SpriteInstance
	xyPair position;		//	float *2;
	float rotation = 0.f;	//	float
	float z = 0.f;			//	float
};