#include "sprite_sheet.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../dependancies/stb_image.h"

#include <fstream>
#include <filesystem>

glm::vec2 SpriteSheet::GetSpriteDimensions() const {
	return { m_SpriteUniformUVs.u1, m_SpriteUniformUVs.v1 };
}


glm::vec2 SpriteSheet::GetCalculatedSpriteOffsets(
	int _spriteIndex
) const {
	DEBUG_ASSERT(_spriteIndex >= 0 && _spriteIndex < m_SpriteCountPerCol * m_SpriteCountPerCol, "Sprite index out of bounds.");
	
	int xCoord = _spriteIndex % m_SpriteCountPerRow;
	int yCoord = _spriteIndex / m_SpriteCountPerRow;

	return { xCoord * m_SpriteUniformUVs.u1, yCoord * m_SpriteUniformUVs.v1 };
}

SpriteSheet::SpriteSheet(
	const std::string& _locationOfImageOrConfigFile,
	const std::string& _sheetName,
	const Shader* _preferredShader,
	int _spritesPerRow_IGNORED_IF_LOADING_CONFIG_FILE,
	int _spritesPerCol_IGNORED_IF_LOADING_CONFIG_FILE
) 
	: 
		m_SheetName(_sheetName),
		m_Shader(_preferredShader)
{

	int ReturnCode = DetermineLoadingMethodFromGivenPath(_locationOfImageOrConfigFile);
	DEBUG_ASSERT(ReturnCode != c_ErrorInLoadingMethodReturnCode,
		"Incorrect file type set for sheet with name [%s]. Must have an extension .png or .cfg to work.", GetName().c_str());

	if (ReturnCode == c_StandardImageLoadingMethodReturnCode) {
		StandardImageLoadingMethod(
			_locationOfImageOrConfigFile.c_str(),
			_spritesPerRow_IGNORED_IF_LOADING_CONFIG_FILE,
			_spritesPerCol_IGNORED_IF_LOADING_CONFIG_FILE
		);
	}
	else if (ReturnCode == c_ConfigFileLoadingMethodReturnCode) {
		ConfigurationPairLoadingMethod(
			_locationOfImageOrConfigFile.c_str()
		);
	}
}


void SpriteSheet::TransformIndicesToUVRegionArray(
	const int* _indexArray,
	const int _indexArraySize,
	std::vector<UVRegion>& OUT_uvRegionArray
) const {

	DEBUG_ASSERT(_indexArray != nullptr, "Nullptr sent to TransformIndicesToUVregionArray() in sprite sheet with name [%s].", GetName().c_str());
	DEBUG_ASSERT(_indexArraySize >= 0, "0 length array passed to TransformIndicesToUVregionArray() in sprite sheet with name [%s].", GetName().c_str());

	OUT_uvRegionArray.clear();


	if (OUT_uvRegionArray.capacity() < _indexArraySize) {
		OUT_uvRegionArray.reserve(_indexArraySize);
	}

	OUT_uvRegionArray.resize(_indexArraySize);


	const UVRegion* UVRegionArray = GetUVRegionArray();
	for (size_t i = 0; i < static_cast<int>(_indexArraySize); i++) {
		
		const int SpriteIndex = _indexArray[i];

		DEBUG_ASSERT(SpriteIndex < m_UVregionsFromConfigFile.size(), "Indexing out of bounds in UVRegion array in sprite sheet with name [%s].", GetName().c_str());

		OUT_uvRegionArray[i] = UVRegionArray[SpriteIndex];
	}
}


int SpriteSheet::DetermineLoadingMethodFromGivenPath(
	const std::string& _pathFromConstructor
) {

	std::string FileExtention;

	
	int FirstDotFromEndToBeginning = _pathFromConstructor.rfind('.');

	int FirstSlash = _pathFromConstructor.rfind('/');
	int FirstBackslash = _pathFromConstructor.rfind('\\');
	
	if (FirstSlash > FirstDotFromEndToBeginning
		|| FirstBackslash > FirstDotFromEndToBeginning) {
		DEBUG_ASSERT(0, "Strange file path given to sheet with name [%s].", GetName().c_str());
		m_Type = SpriteSheetType::ErrorLoading;
		return c_ErrorInLoadingMethodReturnCode;
	}

	FileExtention = _pathFromConstructor.substr(FirstDotFromEndToBeginning,
		_pathFromConstructor.length() - FirstDotFromEndToBeginning);


	if (!FileExtention.compare(".png")) {
		m_Type = SpriteSheetType::IndexBased;
		return c_StandardImageLoadingMethodReturnCode;
	}
	else if (!FileExtention.compare(".cfg")) {
		m_Type = SpriteSheetType::DirectUV;
		return c_ConfigFileLoadingMethodReturnCode;
	}

	m_Type = SpriteSheetType::ErrorLoading;
	return c_ErrorInLoadingMethodReturnCode;
}


void SpriteSheet::ConfigurationPairLoadingMethod(
	const char* _pathToConfigFile
) {
	std::fstream File;
	File.open(_pathToConfigFile, std::ios::in);

	DEBUG_ASSERT(File.is_open(), "[%s] > File couldn't be opened.", _pathToConfigFile);


	std::string Line;
	Line.reserve(200);
	
	std::getline(File, Line);


	size_t FirstSlash = Line.find('\"');
	size_t LastSlash = Line.rfind('\"');

	DEBUG_ASSERT(
		Line.find("relpath=") != std::string::npos ||
		FirstSlash != std::string::npos ||
		LastSlash != std::string::npos, 
		"[%s] > Configuration file malformed.", _pathToConfigFile);
	
	
	std::string TextureImagePath = Line.substr(
		FirstSlash + 1,
		LastSlash - FirstSlash - 1
		);

	LoadImageInTexture(TextureImagePath.c_str());

	std::string AssetName;
	AssetName.resize(100, '\0');
	while (std::getline(File, Line)) {

		if (Line.empty()) continue;


		if (sscanf(Line.c_str(), "[%100[^]]]", AssetName.data()) == 1) {
			m_UVregionNamesFromConfigFile.emplace_back(AssetName);
			continue;
		}

		int xOffset, yOffset, AssetWidth, AssetHeight;
		DEBUG_ASSERT(sscanf(Line.c_str(), "x%dy%dw%dh%d", &xOffset, &yOffset, &AssetWidth, &AssetHeight) == 4,
			"[%s] > Configuration file malformed.", _pathToConfigFile);

		m_UVregionsFromConfigFile.emplace_back(
			xOffset / static_cast<float>(m_SheetWidth),
			yOffset / static_cast<float>(m_SheetHeight),
			(xOffset + AssetWidth) / static_cast<float>(m_SheetWidth),
			(yOffset + AssetHeight) / static_cast<float>(m_SheetHeight)
		);
	}
}


void SpriteSheet::StandardImageLoadingMethod(
	const char* _pathFromConstructor,
	int _spritesPerRow,
	int _spritesPerCol
) {

	m_SpriteCountPerRow = _spritesPerRow;
	m_SpriteCountPerCol = _spritesPerCol;

	LoadImageInTexture(_pathFromConstructor);

	//	Keep it within [0, 1]
	m_SpriteUniformUVs.u0 = 0;	m_SpriteUniformUVs.v0 = 0;
	m_SpriteUniformUVs.u1 = static_cast<float>(1.f / m_SpriteCountPerRow);
	m_SpriteUniformUVs.v1 = static_cast<float>(1.f / m_SpriteCountPerCol);
}


void SpriteSheet::LoadImageInTexture(
	const char* _pathToFile
) {
	unsigned char* imageData = nullptr;
	int nChannels;
	imageData = stbi_load(
		_pathToFile,
		&m_SheetWidth,
		&m_SheetHeight,
		&nChannels,
		4
	);

	if (!imageData) {
		// unfixable, crash here
		std::cout << stbi_failure_reason();
		throw std::exception(stbi_failure_reason());
	}

	glGenTextures(1, &m_TextureBufferID);
	glBindTexture(GL_TEXTURE_2D, m_TextureBufferID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_SheetWidth, m_SheetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData));

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
}


const UVRegion* SpriteSheet::GetUVRegionByName(
	const char* _assetName
) const {

	for (size_t i = 0; i < m_UVregionNamesFromConfigFile.size(); i++) {

		if (!strcmp(_assetName, m_UVregionNamesFromConfigFile[i].data())) {
			return &m_UVregionsFromConfigFile[i];
		}
	}
	return nullptr;
}


void SpriteSheet::DestroyGLTextureObject() { glDeleteTextures(1, &m_TextureBufferID); }
const std::string& SpriteSheet::GetName() const { return m_SheetName; }
SpriteSheet::SpriteSheet() {}



UVRegion::UVRegion(
	float _u0, float _v0,
	float _u1, float _v1
)
	: 
	u0(_u0), v0(_v0),
	u1(_u1), v1(_v1)
{}


bool UVRegion::operator==(const UVRegion& other) const {
	
	return fEqual(u0, other.u0)
		&& fEqual(v0, other.v0)
		&& fEqual(u1, other.u1)
		&& fEqual(v1, other.v1);
}





