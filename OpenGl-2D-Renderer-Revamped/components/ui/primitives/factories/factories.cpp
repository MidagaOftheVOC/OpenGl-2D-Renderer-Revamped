#include "pane_factory.h"



/*		PANE	FACTORY		*/

PaneFactory::PaneFactory() {}


void PaneFactory::AddSkin(
	const PaneSkin& _skin,
	const std::string& _name
) {
	m_LoadedSkins[_name] = _skin;
}


const PaneSkin* PaneFactory::GetSkinByName(
	const char* _name
) const {

	auto Iterator = m_LoadedSkins.find(_name);

	if (Iterator != m_LoadedSkins.end()) {
		return &Iterator->second;	//	nice
	}


	if (m_IsDefaultSkinSet) {
		DEBUG_WARN(0, "PaneFactory cannot find skin with name [%s]. Returning default skin...", _name);
		Iterator = m_LoadedSkins.find(m_DefaultSkinName.c_str());

		if (Iterator == m_LoadedSkins.end()) {
			DEBUG_ASSERT(0, "PaneFactory cannot find the default skin. Crash here...");
			return nullptr;
		}

		return &Iterator->second;
	}

	DEBUG_ASSERT(0, "PaneFactory cannot find skin with name [%s]. Crash here...", _name);
	return nullptr;
}


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


	Pane self({_width, _height}, _cornerLength);
	const PaneSkin* Skin = GetSkinByName(_skinName);

	self.SetSpriteInformation(Skin->m_SIArray);
	
	return self;
}


void PaneFactory::SetDefaultSkinName(
	const char* _name
) {
	m_IsDefaultSkinSet = true;
	m_DefaultSkinName = _name;
}

