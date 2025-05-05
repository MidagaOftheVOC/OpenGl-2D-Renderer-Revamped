#pragma once



/*
* Class: Drawable
* Purpose: Data that can be drawn to the screen. Should represent the graphical side
* of objects.
* 
* More specifically, this will include information locating a sprite within a spritesheet
* or otherwise information about where to find what to draw with what dimensions.
* 
* 
* Common sprites will use a base 100/100px square vert buffer, and resize from there.
*/


#include "../common/common.h"

#include "sprite_sheet.h"

class Drawable {

private:

	const SpriteRegion* m_SpriteRegion = nullptr;
	
	int m_ForcedXvalue = -1;
	int m_ForcedYvalue = -1;

public:

	Drawable() {}

	Drawable(
		const SpriteRegion* _spriteRegion
	);



	const SpriteRegion* GetSpriteRegion() const;


	bool IsGood();


	bool HasForcedDimensions();


	void SetForcedDimentionsInPixels(
		int _forcedXvalue,
		int _forcedYvalue
	);
};
