#include "resource_service.h"

static const char* g_DefaultFontName = "cyrillic";

void ResourceService::LoadShader(
	const std::string& _locationShaderFile,
	const std::string& _shaderName
) {
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
	Font self = Font(GetSpriteSheetByName(g_DefaultFontName), g_DefaultFontName);
	std::vector<unsigned short> offsets;
	offsets.resize(80, 20);
	self.Init(
		U"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЬЮЯЍ",
		U"абвгдежзийклмнопрстуфхцчшщъьюяѝ",
		U".,+-!?;:&><#/",
		offsets.data(),
		80
	);

	m_Fonts.emplace_back(std::move(self));
	m_DefaultTextOptions.font = &m_Fonts[0];


	m_UIBatch.InitialiseBuffers();
	m_UIBatch.AddSheetToBatch(GetSpriteSheetByName(c_SpecialUISheetName));
	m_UIBatch.AddSheetToBatch(GetSpriteSheetByName("cyrillic"));
	m_UIBatch.BufferUBOs();

	BackgroundSkin defSkin;
	defSkin.cornerLengthPx = 20;
	defSkin.name = "default";
	for (int i = 0; i < 9; i++) {
		defSkin.instanceArray[i] = m_UIBatch.GetSprite(c_SpecialUISheetName, BackgroundSkin::DEFAULT_BG_SUBSPRITE_NAMES[i]);
	}
	AddBackgroundSkin(defSkin);
}

const BackgroundSkin* ResourceService::GetBgSkinByName(
	const char* _name
) const {
	size_t paneCount = m_PaneSkins.size();
	if (paneCount == 0) {
		DEBUG_ASSERT(0, "GetBgSkingByName() called when no skins were loaded.");
		return nullptr;
	}

	if (!_name) {
		return &m_PaneSkins[0];
	}

	for (size_t i = 0; i < paneCount; i++) {
		if (
			strcmp(_name, m_PaneSkins[i].name.c_str()) == 0
			) {
			return &m_PaneSkins[i];
		}
	}

	DEBUG_WARN(0, "GetBgSkinByName() for name [%s] returned nullptr.", _name);
	return nullptr;
}

void ResourceService::AddBackgroundSkin(
	const BackgroundSkin& _skin
) {
	m_PaneSkins.emplace_back(_skin);
}

const Font* ResourceService::GetFontByName(
	const char* name
) const {
	size_t fontCount = m_Fonts.size();
	if (fontCount == 0) {
		DEBUG_ASSERT(0, "GetFontByName() called when no skins were loaded.");
		return nullptr;
	}

	if (!name) {
		return &m_Fonts[0];
	}

	for (size_t i = 0; i < fontCount; i++) {
		if (
			strcmp(name, m_Fonts[i].GetName().c_str()) == 0
			) {
			return &m_Fonts[i];
		}
	}

	DEBUG_WARN(0, "GetFontByName() for name [%s] returned nullptr.", name);
	return nullptr;
}