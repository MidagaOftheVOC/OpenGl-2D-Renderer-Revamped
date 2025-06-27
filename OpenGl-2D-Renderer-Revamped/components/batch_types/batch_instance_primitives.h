#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"



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


//	Position information struct
//	Note: Position here is relative to where the drawcall for the batch was made
struct PositionInformation {

	PositionInformation(
		float _xRelPosition,
		float _yRelPosition
	);


	void UpdateValues(
		float _xChange,
		float _yChange
	);

public:	//	Get offsets for setting up formats

	static size_t GetPositionPairValuesOffset() { return offsetof(PositionInformation, m_xRelativePosition); }
	static size_t GetRotationValueOffset() { return offsetof(PositionInformation, m_RotationRad);}

private:	

	float m_xRelativePosition = 0.f;
	float m_yRelativePosition = 0.f;

	float m_RotationRad = 0.f;
};


struct DimensionInformation {



};

