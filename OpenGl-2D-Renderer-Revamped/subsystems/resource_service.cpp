#include "resource_service.h"

void ResourceService::LoadShader(
	const std::string& _locationShaderFile,
	const std::string& _shaderName
) {
	//	Special common shader used only by Text objects
	if (!_shaderName.compare(c_SpecialTextShaderName)) {
		m_TextRenderingShader = Shader(
			_locationShaderFile,
			_shaderName
		);
		return;
	}

	m_Shaders.emplace_back(
		_locationShaderFile,
		_shaderName
	);
}


void ResourceService::LoadSpriteSheet(
	const std::string& _locationRawImage,
	const std::string& _sheetName,
	const Shader* _preferredShader,
	int _spritesPerRow,
	int _spritesPerCol
) {
	m_Sheets.emplace_back(
		_locationRawImage,
		_sheetName,
		_preferredShader,
		_spritesPerRow,
		_spritesPerCol
	);
}


const SpriteSheet* ResourceService::GetSpriteSheetByName(
	const char* _spriteSheetName
) {
	if (!_spriteSheetName) return nullptr;
	size_t len = m_Sheets.size();
	for (size_t i = 0; i < len; i++) {
		if (
			strcmp(_spriteSheetName, m_Sheets[i].GetName().c_str()) == 0
			) {
			return &m_Sheets[i];
		}
	}
	DEBUG_WARN(0, "GetSpriteSheetByName() for name [%s] returned nullptr.", _spriteSheetName);
	return nullptr;
}


const Shader* ResourceService::GetShaderByName(
	const char* _shaderName
) {
	if (!_shaderName) return nullptr;
	size_t len = m_Shaders.size();
	for (size_t i = 0; i < len; i++) {
		if (
			strcmp(_shaderName, m_Shaders[i].GetName().c_str()) == 0
			) {
			return &m_Shaders[i];
		}
	}
	DEBUG_WARN(0, "GetShaderByName() for name [%s] returned nullptr.", _shaderName);
	return nullptr;
}


void ResourceService::UploadShaderParameters(
	const char* _location,
	const char* _shaderName
) {
	m_ShaderLoadQueue.emplace_back(_location, _shaderName);
}


void ResourceService::UploadSpriteSheetParameters(
	const char* _locationRawImage,
	const char* _sheetName,
	const char* _preferredShader,
	int _spritesPerRow,
	int _spritesPerCol
) {
	m_SpriteSheetLoadQueue.emplace_back(
		_locationRawImage,
		_sheetName,
		_preferredShader,
		_spritesPerRow,
		_spritesPerCol
	);
}


void ResourceService::StartLoadingProcess() {

	m_ShaderArray.clear();
	m_SpriteSheetArray.clear();

	if (m_ShaderArray.capacity() <= m_ShaderLoadQueue.size()) {
		m_ShaderArray.reserve(m_ShaderLoadQueue.size());
	}

	for (size_t i = 0; i < m_ShaderLoadQueue.size(); i++) {
		ShaderLoadingParameters& params = m_ShaderLoadQueue[i];
		LoadShader(
			std::string(params.m_LocationOfShaderFile),
			std::string(params.m_ShaderName)
		);
	}
	m_ShaderLoadQueue.clear();

	
	if (m_SpriteSheetArray.capacity() <= m_SpriteSheetLoadQueue.size()) {
		m_SpriteSheetArray.reserve(m_SpriteSheetLoadQueue.size());
	}

	for (size_t i = 0; i < m_SpriteSheetLoadQueue.size(); i++) {
		SpriteSheetLoadingParameters& params = m_SpriteSheetLoadQueue[i];

		if (!params.m_PreferredShaderName.compare(c_SpecialTextShaderName)) {
			
			LoadSpriteSheet(
				std::string(params.m_LocationOfImage),
				std::string(params.m_SheetName),
				&GetTextShader(),
				params.m_SpritesPerRow,
				params.m_SpritesPerCol
			);
		}

		LoadSpriteSheet(
			std::string(params.m_LocationOfImage),
			std::string(params.m_SheetName),
			GetShaderByName(params.m_PreferredShaderName.c_str()),
			params.m_SpritesPerRow,
			params.m_SpritesPerCol
		);
	}
	m_SpriteSheetLoadQueue.clear();

	LoadDefaultVariables();
}