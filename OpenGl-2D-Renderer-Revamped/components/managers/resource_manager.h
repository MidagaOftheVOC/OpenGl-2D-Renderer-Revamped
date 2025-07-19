#pragma once

#include "../../common/common.h"


#include "../shader.h"
#include "../sprite_sheet.h"

#include "../ui/text.h"
#include "../ui/primitives/factories/pane_factory.h"



#include <map>



static const char* g_ResConfigNewEntityKeyNames[] = {
	"shader_directory",
	"sheet_directory",
	"img_directory",
	
	"shader",
	"sheet",
	"font",

	"pane",
	"button",
	"tex_button"
};

static const char* g_ResConfigKeyNames[] = {
	"file",
	"img",
	"cfg",
	"shader_name",
	"w", "h",
	"offsets",
	"unknown_symbol_replacement_index",
	"space_index",


	"pane_corner_len",
	"pane_ul",
	"pane_ur",
	"pane_ll",
	"pane_lr",

	"pane_left",
	"pane_right",
	"pane_upper",
	"pane_lower",

	"pane_mid",

	"button_corner_len",
	"button_ul",
	"button_ur",
	"button_ll",
	"button_lr",

	"button_left",
	"button_right",
	"button_upper",
	"button_lower",

	"button_mid",

	"sprite"
};


/*

Contains no validation for entity key composition and value correctness

*/

class ResourceManager {

	fs::path m_PathToMain;
	fs::path m_DirectoryOfLastResConfigFile;

	fs::path m_DirectoryOfSpriteSheetConfigFiles;
	fs::path m_DirectoryOfSpriteSheetImageFiles;

	fs::path m_DirectoryOfShaders;

	fs::path m_DirectoryOfFonts;

private:	//	Core assets

	std::vector<SpriteSheet> m_SpriteSheets;


	std::vector<Shader>	m_Shaders;

	//	UI-related assets

	std::vector<Font> m_Fonts;


private:	//	UI primitive factories

	PaneFactory m_PaneFactory;

private:

	static const std::string s_TextShaderName;

public:

	ResourceManager() {}
	

	bool LoadAssetsFromResourceConfigFile(
		const char* _rcfgFileLocation
	);

private:

	std::string_view GetKeyFromToken(
		std::string_view _token
	) const;


	std::string_view GetValueFromToken(
		std::string_view _token
	) const;

};




