#include "pane_factory.h"

Pane PaneFactory::CreatePane(
	float _width,
	float _height,
	float _cornerLength,
	const char* _skinName
) const {
	DEBUG_ASSERT(_width > 1.f, "Non-positive width passed to Pane object.");
	DEBUG_ASSERT(_height > 1.f, "Non-positive height passed to Pane object.");
	DEBUG_ASSERT(_cornerLength > 1.f, "Non-positive corner length passed to Pane object.");
	DEBUG_ASSERT(_skinName != nullptr, "No name passed to PaneFactory::CreatePane().");


	Pane self({ _width, _height }, _cornerLength);
	const PaneSkin* Skin = GetResService()->GetSkinByName(_skinName);

	self.SetSpriteInformation(Skin->m_SIArray);

	return self;
}