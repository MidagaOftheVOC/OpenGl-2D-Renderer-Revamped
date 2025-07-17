#include "resource_manager.h"


//	DTO-like structs

struct ShaderLoadParams{
	const std::string ShaderLogicalName;
	const std::string ShaderFileLocation;

	ShaderLoadParams(const std::string& ShaderLogicalName, const std::string& ShaderFileLocation)
		: ShaderLogicalName(ShaderLogicalName), ShaderFileLocation(ShaderFileLocation)
	{}
};


struct SheetLoadParams {
	const std::string SheetLogicalName;
	const std::string TextureLocation;
	const std::string SpriteConfigLocation;
	const std::string RequiredShaderLogicalName;

	SheetLoadParams(const std::string& SheetLogicalName, const std::string& TextureLocation, const std::string& SpriteConfigLocation, const std::string& RequiredShaderLogicalName)
		: SheetLogicalName(SheetLogicalName), TextureLocation(TextureLocation), SpriteConfigLocation(SpriteConfigLocation), RequiredShaderLogicalName(RequiredShaderLogicalName)
	{}
};


struct FontLoadParams {
	const std::string FontLogicalName;
	const std::string FontTextureLocation;
	const uint16_t RowSpriteCount;
	const uint16_t ColSpriteCount;
	std::vector<unsigned char> GlyphOffsets;
	const uint16_t UnknownSymbolReplacementIndex;
	const uint16_t SpaceIndex;

	FontLoadParams(const std::string& FontLogicalName, const std::string& FontTextureLocation, const uint16_t& RowSpriteCount, const uint16_t& ColSpriteCount, const std::vector<unsigned char>& GlyphOffsets, const uint16_t& UnknownSymbolReplacementIndex, const uint16_t& SpaceIndex)
		: FontLogicalName(FontLogicalName), FontTextureLocation(FontTextureLocation), RowSpriteCount(RowSpriteCount), ColSpriteCount(ColSpriteCount), GlyphOffsets(GlyphOffsets), UnknownSymbolReplacementIndex(UnknownSymbolReplacementIndex), SpaceIndex(SpaceIndex)
	{}
};


struct GenericUI9PatchSkinLoadParams {

};

enum class InputType {
	Directory = 0,
	Shader = 1,
	Sheet = 2,
	Font = 3,
	UISkin = 4,

	None = -1
};


bool ResourceManager::LoadAssetsFromResourceConfigFile(
	const char* _rcfgFileLocation
) {
	FileReader File = FileReader::OpenFile(_rcfgFileLocation);
	if (!File.IsFileOpened()) return false;

	Tokeniser Tokens;

	std::string Line;
	InputType CurrentInput = InputType::None;
	while (File.ReadNextContentLine(Line)) {
		Tokens.LoadString(Line);
		

	}

	return true;
}

