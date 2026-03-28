#include "resource_manager.h"



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




const size_t ShaderLoadParams::s_RequiredKeyCount = 2;
const size_t SheetLoadParams::s_RequiredKeyCount = 4;
const size_t  FontLoadParams::s_RequiredKeyCount = 7;







bool ResourceManager::LoadAssetsFromResourceConfigFile(
	const char* _rcfgFileLocation
) {
	std::ifstream FileStream(_rcfgFileLocation, std::ios::in);

	DEBUG_ASSERT(FileStream.is_open(), "Rconfig file cannot be opened at [%s].", _rcfgFileLocation);

	m_LastLoadedRconfigFile.assign(
		std::istreambuf_iterator<char>(FileStream),
		std::istreambuf_iterator<char>()
	);

	m_DirectoryOfLastResConfigFile = fs::path(_rcfgFileLocation);
	m_IndexOfLastReadCharacterFromRconfigFile = 0;

	Trim(m_LastLoadedRconfigFile);

	std::string_view CurrBlock;

	while (!(CurrBlock = GetNextEntityBlock()).empty()) {

		while (true) {

			size_t 

		}


	}

	return true;
}


void ResourceManager::ParseIndividualBlock(
	std::string_view _blockString
) {
	BlockDescriptor SelfType = DetermineBlockType(_blockString);

	m_ParsedEntityBlocks.emplace_back();

	m_ParsedEntityBlocks.back().m_BlockType = SelfType;

	std::string Line;
	while()


}


std::string_view ResourceManager::GetNextEntityBlock() {
	size_t FirstBracket = m_IndexOfLastReadCharacterFromRconfigFile;
	size_t LastBracket = m_IndexOfLastReadCharacterFromRconfigFile;
	size_t FirstCharacterOfEntityName = m_IndexOfLastReadCharacterFromRconfigFile;


	FirstBracket = m_LastLoadedRconfigFile.find("{", m_IndexOfLastReadCharacterFromRconfigFile);
	if(FirstBracket == std::string::npos){
		return std::string_view();
	}


	LastBracket = m_LastLoadedRconfigFile.find("}", FirstBracket);
	DEBUG_ASSERT(LastBracket != std::string::npos,
		"Malformed entity block in resource config (.rcfg) file [%s].", m_DirectoryOfLastResConfigFile.string().c_str());
	if (LastBracket == std::string::npos) {
		return std::string_view();
	}

	bool IsInEntityName = false;

	for (size_t i = 0; i < m_LastLoadedRconfigFile.size(); i++) {
		const char CurrCharacter = m_LastLoadedRconfigFile[FirstBracket - i];

		if (std::isalnum(CurrCharacter) || CurrCharacter == '_') {
			IsInEntityName = true;
		}
		else if (IsInEntityName) {
			FirstCharacterOfEntityName = FirstBracket - i + 1;
			break;
		}
	}

	m_IndexOfLastReadCharacterFromRconfigFile = LastBracket + 1;
	return std::string_view(m_LastLoadedRconfigFile.data() + FirstCharacterOfEntityName, LastBracket - FirstCharacterOfEntityName + 1);
}


BlockDescriptor ResourceManager::DetermineBlockType(
	const std::string_view _blockString
) const {
	std::string_view BlockTypeString = _blockString.substr(0, _blockString.find("{") - 1);
	
	if (!BlockTypeString.compare("config"))		return BlockDescriptor::Config;
	if (!BlockTypeString.compare("shader"))		return BlockDescriptor::Shader;
	if (!BlockTypeString.compare("sheet"))		return BlockDescriptor::SpriteSheet;
	if (!BlockTypeString.compare("font"))		return BlockDescriptor::Font;
	if (!BlockTypeString.compare("pane"))		return BlockDescriptor::PaneSkin;
	if (!BlockTypeString.compare("button"))		return BlockDescriptor::Button;
	if (!BlockTypeString.compare("texbutton"))	return BlockDescriptor::TexButton;

	if (!BlockTypeString.empty()) return BlockDescriptor::None;	// error?

	return BlockDescriptor::Error;
}


void ResourceManager::Trim(
	std::string& _rconfigString
) const {
	size_t LastCommentIndex = _rconfigString.find("//", 0);

	//	Prune comments
	while (LastCommentIndex != std::string::npos) {
		size_t NewLineIndex = _rconfigString.find('\n', LastCommentIndex);

		if (NewLineIndex == std::string::npos) {
			_rconfigString.erase(LastCommentIndex);
			break;
		}

		_rconfigString.erase(LastCommentIndex, NewLineIndex - LastCommentIndex);
		
		LastCommentIndex = _rconfigString.find("//", LastCommentIndex);
	}

	
	//	Prune empty lines
	
	/*bool IsPrevCharacterANewLine = false;
	for (size_t i = 0; i < _rconfigString.size(); i++) {
		const char CurrCharacter = _rconfigString[i];

		if (CurrCharacter == '\n') {

			if (IsPrevCharacterANewLine) {
				_rconfigString.erase(_rconfigString.begin() + i);
				i--;
				continue;
			}
			else {
				IsPrevCharacterANewLine = true;
			}

		}

	}*/
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


