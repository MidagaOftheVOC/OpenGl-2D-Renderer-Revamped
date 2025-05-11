#pragma once



/*
* Class: Drawable
* Purpose: Data that can be drawn to the screen. Should represent the graphical side
* of objects.
* 
* NOTE:	If forced values in Drawables are needed, it's probably better to clone the
*		Drawable object, store it, and set its values to initiate Drawcalls with.
* 
*/


#include "../common/common.h"

#include "sprite_sheet.h"

class Drawable {

private:

	const SpriteSheet* m_SpriteSheet = nullptr;
	const int m_SpriteIndexInSheet = -1;
	int m_ForcedXvalue = -1;
	int m_ForcedYvalue = -1;

public:


	Drawable() {}

	Drawable(
		const SpriteSheet* _spriteSheet,
		int _spriteIndex
	);


	const SpriteSheet* GetAsociatedSpriteSheet() const { return m_SpriteSheet; }
	
	int GetSpriteIndex()  const { return m_SpriteIndexInSheet; }
	int GetForcedXValue() const { return m_ForcedXvalue; }
	int GetForcedYValue() const { return m_ForcedYvalue; }

	bool IsGood() const;


	bool HasForcedDimensions() const;


	void SetForcedDimentionsInPixels(
		int _forcedXvalue,
		int _forcedYvalue
	);
};
