#pragma once

#include "../../common/common.h"

//	Instance-related primitive structs for sets of data

struct SpriteInformation {

	SpriteInformation() {}
	
	SpriteInformation(
		uint32_t _sheetIndex,
		uint32_t _spriteIndex
	);

	uint32_t GetSheetIndex() const;
	uint32_t GetSpriteIndex() const;
	
	void SetSheetIndex(uint32_t sheetIndex);
	void SetSpriteIndex(uint32_t spriteIndex);

private:

	uint32_t data = 0;

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

	FullSprite() {}

	FullSprite(
		const SpriteInstance& newInstance,
		float x, float y
	) :
		instance(newInstance)
	{
		position.x = x;
		position.y = y;
	}

};