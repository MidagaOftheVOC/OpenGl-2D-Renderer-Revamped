#pragma once

#include "../types/pane.h"

#include <unordered_map>

struct PaneSkin {
	SpriteInformation m_SIArray[9];
};


/*
PaneFactory will load PaneSkin objects and keep track of them using their names.
It will create Pane objects with a skin requested by name.

If a skin is not found, the first one loaded will be used as default.
If there is none loaded, we crash because something is seriously off the rails.
*/
class PaneFactory {

	std::unordered_map<std::string, PaneSkin> m_LoadedSkins;


	bool m_IsDefaultSkinSet = false;
	std::string m_DefaultSkinName;

public:

	PaneFactory();


	void AddSkin(
		const PaneSkin& _skin,
		const std::string& _name
	);


	Pane CreatePane(
		float _width,
		float _height,
		float _cornerLength,
		const char* _skinName
	) const;

private:

	const PaneSkin* GetSkinByName(
		const char* _name
	) const;

public:

	void SetDefaultSkinName(
		const char* _name
	);
};

