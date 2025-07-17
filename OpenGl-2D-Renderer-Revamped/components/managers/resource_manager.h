#pragma once

#include "../../common/common.h"


#include "../shader.h"
#include "../sprite_sheet.h"

#include "../ui/text.h"


class ResourceManager {

	fs::path m_PathToMain;
	fs::path m_DirectoryOfLastResConfigFile;

	fs::path m_DirectoryOfSpriteSheetConfigFiles;
	fs::path m_DirectoryOfSpriteSheetImageFiles;

	fs::path m_DirectoryOfShaders;

private:	//	Core assets

	std::vector<SpriteSheet> m_SpriteSheets;


	std::vector<Shader>	m_Shaders;

	//	UI-related assets

	std::vector<Font> m_Fonts;


private:	//	UI primitive factories

	PaneFactory m_PaneFactory;

public:

	ResourceManager() {}
	

	bool LoadAssetsFromResourceConfigFile(
		const char* _rcfgFileLocation
	);

private:



};




