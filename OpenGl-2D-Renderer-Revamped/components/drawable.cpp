#include "drawable.h"












Drawable::Drawable(
	const SpriteRegion* _spriteRegion
) {
	m_SpriteRegion = _spriteRegion;
}


bool Drawable::IsGood() {

	// TODO: bug check

	return true;
}


bool Drawable::HasForcedDimensions() {
	return m_ForcedXvalue != -1 || m_ForcedYvalue != -1;
}



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


const SpriteRegion* Drawable::GetSpriteRegion() {
	return m_SpriteRegion;
}



