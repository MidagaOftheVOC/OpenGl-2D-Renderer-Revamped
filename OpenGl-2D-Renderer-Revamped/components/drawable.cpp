#include "drawable.h"












Drawable::Drawable(
	const SpriteSheet* _spriteSheet,
	int _spriteIndex
)
	: m_SpriteIndexInSheet(_spriteIndex),
	m_SpriteSheet(_spriteSheet)
{}


bool Drawable::IsGood() const {

	// TODO: bug check

	return true;
}


bool Drawable::HasForcedDimensions() const {
	return m_ForcedXvalue != -1 || m_ForcedYvalue != -1;
}


//		UNUSED
void Drawable::SetForcedDimentionsInPixels(
	int _forcedXvalue,
	int _forcedYvalue
) {

	if (_forcedXvalue >= 0 && _forcedXvalue == -1) {
		m_ForcedXvalue = _forcedXvalue;
	}

	if (_forcedYvalue >= 0 && _forcedYvalue == -1) {
		m_ForcedYvalue = _forcedYvalue;
	}
}


