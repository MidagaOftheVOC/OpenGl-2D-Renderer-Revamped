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
	m_Shaders.clear();
	m_Sheets.clear();

	if (m_Shaders.capacity() <= m_ShaderLoadQueue.size()) {
		m_Shaders.reserve(m_ShaderLoadQueue.size());
	}

	for (size_t i = 0; i < m_ShaderLoadQueue.size(); i++) {
		ShaderLoadingParameters& params = m_ShaderLoadQueue[i];
		LoadShader(
			std::string(params.m_LocationOfShaderFile),
			std::string(params.m_ShaderName)
		);
	}
	m_ShaderLoadQueue.clear();

	
	if (m_Sheets.capacity() <= m_SpriteSheetLoadQueue.size()) {
		m_Sheets.reserve(m_SpriteSheetLoadQueue.size());
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
			continue;
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

void ResourceService::LoadDefaultVariables() {
	m_DefaultFont = Font(GetSpriteSheetByName("test_font"), "cyrillic");

	unsigned short off[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, };
	m_DefaultFont.Init(
		U"абвгдежзийклмнопрстуфхцчшщъьюяѝ .,+-!?;:&><#/",
		off,
		50
	);

	m_DefaultTextOptions.m_Font = &m_DefaultFont;
}

const PaneSkin* ResourceService::GetSkinByName(
	const char* _name
) const {
	if (!_name) return nullptr;
	size_t len = m_PaneSkins.size();
	for (size_t i = 0; i < len; i++) {
		if (
			strcmp(_name, m_PaneSkins[i].m_Name.c_str()) == 0
			) {
			return &m_PaneSkins[i];
		}
	}
	DEBUG_WARN(0, "GetShaderByName() for name [%s] returned nullptr.", _name);
	return nullptr;
}

void ResourceService::AddSkin(
	const PaneSkin& _skin
) {
	m_PaneSkins.emplace_back(_skin);
}