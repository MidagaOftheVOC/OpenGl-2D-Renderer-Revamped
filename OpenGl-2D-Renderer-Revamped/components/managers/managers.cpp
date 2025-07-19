#include "resource_manager.h"




const std::string ResourceManager::s_TextShaderName = "__TEXT_SHADER__";




//	DTO-like structs

struct ShaderLoadParams{
	const std::string ShaderLogicalName;
	const std::string ShaderFileLocation;

	static const size_t s_RequiredKeyCount;

	ShaderLoadParams(const std::string_view ShaderLogicalName, const std::string_view ShaderFileLocation)
		: ShaderLogicalName(ShaderLogicalName), ShaderFileLocation(ShaderFileLocation)
	{}
};


struct SheetLoadParams {
	const std::string SheetLogicalName;
	const std::string TextureLocation;
	const std::string SpriteConfigLocation;
	const std::string RequiredShaderLogicalName;

	static const size_t s_RequiredKeyCount;

	SheetLoadParams(const std::string_view SheetLogicalName, const std::string_view TextureLocation, const std::string_view SpriteConfigLocation, const std::string_view RequiredShaderLogicalName)
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

	static const size_t s_RequiredKeyCount;

	FontLoadParams(
		const std::string_view FontLogicalName,
		const std::string_view FontTextureLocation,
		const uint16_t& RowSpriteCount,
		const uint16_t& ColSpriteCount,
		const std::vector<unsigned char>&& GlyphOffsets,
		const uint16_t& UnknownSymbolReplacementIndex,
		const uint16_t& SpaceIndex
	)
		: FontLogicalName(FontLogicalName), 
		FontTextureLocation(FontTextureLocation), 
		RowSpriteCount(RowSpriteCount), 
		ColSpriteCount(ColSpriteCount), 
		GlyphOffsets(std::move(GlyphOffsets)),
		UnknownSymbolReplacementIndex(UnknownSymbolReplacementIndex), 
		SpaceIndex(SpaceIndex)
	{}
};


struct GenericUI9PatchSkinLoadParams {

};



const size_t ShaderLoadParams::s_RequiredKeyCount = 2;
const size_t SheetLoadParams::s_RequiredKeyCount = 4;
const size_t  FontLoadParams::s_RequiredKeyCount = 7;



bool ResourceManager::LoadAssetsFromResourceConfigFile(
	const char* _rcfgFileLocation
) {
	FileReader File = FileReader::OpenFile(_rcfgFileLocation);
	if (!File.IsFileOpened()) return false;

	m_DirectoryOfLastResConfigFile = fs::path(_rcfgFileLocation);


	std::vector<ShaderLoadParams> ShaderParams;
	std::vector<SheetLoadParams> SheetParams;
	std::vector<FontLoadParams> FontParams;


	Tokeniser Tokens;
	std::string Line;
	std::map<std::string, std::string> SetKeys;

	while (File.ReadNextContentLine(Line)) {
		Tokens.LoadString(Line);
		std::string_view EntityToken;
		std::string_view EntityTokenKey;

		if (SetKeys.empty()) {
			EntityToken = Tokens.GetToken(0);
			EntityTokenKey = GetKeyFromToken(EntityToken);
		}

		if (EntityTokenKey == "shader_directory") {
			m_DirectoryOfShaders = m_DirectoryOfLastResConfigFile / fs::path(GetValueFromToken(EntityToken));
			continue;
		}

		if(EntityTokenKey == "sheet_directory"){
			m_DirectoryOfSpriteSheetConfigFiles = m_DirectoryOfLastResConfigFile / fs::path(GetValueFromToken(EntityToken));
			continue;
		}

		if (EntityTokenKey == "img_directory") {
			m_DirectoryOfSpriteSheetImageFiles = m_DirectoryOfLastResConfigFile / fs::path(GetValueFromToken(EntityToken));
			continue;
		}

		if (EntityTokenKey == "font_directory") {
			m_DirectoryOfFonts = m_DirectoryOfLastResConfigFile / fs::path(GetValueFromToken(EntityToken));
			continue;
		}
		
		if (EntityTokenKey == "shader") {
			ShaderParams.emplace_back(GetValueFromToken(Tokens.GetToken(0)), GetValueFromToken(Tokens.GetToken(1)));
			continue;
		}

		if (EntityTokenKey == "sheet") {
			SheetParams.emplace_back(
				GetValueFromToken(Tokens.GetToken(0)),
				GetValueFromToken(Tokens.GetToken(1)),
				GetValueFromToken(Tokens.GetToken(2)),
				GetValueFromToken(Tokens.GetToken(3))
			);
			continue;
		}

		if (EntityTokenKey == "font") {

			size_t ProcessedKeysCount = 0;

			do {
				size_t TokenCount = Tokens.GetTokenCount();
				for (size_t i = 0; i < TokenCount; i++) {
					std::string_view CurrToken = Tokens.GetToken(i);
					SetKeys[GetKeyFromToken(CurrToken).data()] = std::string(GetValueFromToken(CurrToken));
					ProcessedKeysCount++;
				}

			} while (File.ReadNextContentLine(Line) && ProcessedKeysCount < FontLoadParams::s_RequiredKeyCount);

			std::vector<unsigned char> ParsedOffsetArray;

			

		}

	}

	return true;
}


std::string_view ResourceManager::GetKeyFromToken(
	std::string_view _token
) const {
	size_t EqualSignIndex = _token.find("=");
	DEBUG_ASSERT(EqualSignIndex != std::string::npos, "Resource config key is broken, missing '=' sign for token [%s].", _token.data());
	return std::string_view(_token.data(), EqualSignIndex);
}


std::string_view ResourceManager::GetValueFromToken(
	std::string_view _token
) const {
	size_t EqualSignIndex = _token.find("=");
	DEBUG_ASSERT(EqualSignIndex != std::string::npos, "Resource config key is broken, missing '=' sign for token [%s].", _token.data());

	if (EqualSignIndex == std::string::npos) {
		return std::string_view(_token.data(), 0);
	}

	return std::string_view(_token.data() + EqualSignIndex, _token.length() - EqualSignIndex);
}



