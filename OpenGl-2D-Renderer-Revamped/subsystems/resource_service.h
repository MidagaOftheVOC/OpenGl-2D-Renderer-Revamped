#pragma once

#include "../components/batch_types/base_batch.h"

#include "../components/shader.h"
#include "../components/sprite_sheet.h"
#include "../components/ui/font.h"
#include "../components/ui/widget/widget_interface.h"

struct ShaderLoadingParameters {
	const std::string m_ShaderName;
	const std::string m_LocationOfShaderFile;
	ShaderLoadingParameters(
		const char* _location,
		const char* _shaderName
	)
		: m_ShaderName(_shaderName), m_LocationOfShaderFile(_location) {
	}
};

struct SpriteSheetLoadingParameters {
	const std::string m_LocationOfImage;
	const std::string m_SheetName;
	const std::string m_PreferredShaderName;
	int m_SpritesPerRow;
	int m_SpritesPerCol;

	SpriteSheetLoadingParameters(
		const char* _locationRawImage,
		const char* _sheetName,
		const char* _preferredShader,
		int _spritesPerRow,
		int _spritesPerCol
	) :
		m_LocationOfImage(_locationRawImage),
		m_SheetName(_sheetName),
		m_PreferredShaderName(_preferredShader),
		m_SpritesPerRow(_spritesPerRow),
		m_SpritesPerCol(_spritesPerCol)
	{}
};

class ResourceService {

	GLFWwindow* m_MainWindowContext = nullptr;

private:

	std::vector<Shader> m_Shaders;
	std::vector<SpriteSheet> m_Sheets;

	std::vector<Font> m_Fonts;
	std::vector<PaneSkin> m_PaneSkins;


	std::vector<ShaderLoadingParameters> m_ShaderLoadQueue;
	std::vector<SpriteSheetLoadingParameters> m_SpriteSheetLoadQueue;

private:

	Shader m_TextRenderingShader;

	Font m_DefaultFont;
	TextOptions m_DefaultTextOptions;
	Batch m_UIBatch;

public:

	ResourceService() {}

	ResourceService(
		GLFWwindow* win
	) 
		: m_MainWindowContext(win)
	{}

private:

	void LoadShader(
		const std::string& _locationShaderFile,
		const std::string& _shaderName
	);

	void LoadSpriteSheet(
		const std::string& _locationRawImage,
		const std::string& _sheetName,
		const Shader* _preferredShader,
		int _spritesPerRow,
		int _spritesPerCol
	);

	void LoadDefaultVariables();

public:

	void UploadShaderParameters(
		const char* _location,
		const char* _shaderName
	);

	void UploadSpriteSheetParameters(
		const char* _locationRawImage,
		const char* _sheetName,
		const char* _preferredShader,
		int _spritesPerRow,
		int _spritesPerCol
	);

	void StartLoadingProcess();

public:

	void AddBgSkin(
		const PaneSkin& _skin
	);

public:

	const Shader* GetShaderByName(
		const char* _shaderName
	);

	const SpriteSheet* GetSpriteSheetByName(
		const char* _spriteSheetName
	);

	const PaneSkin* GetSkinByName(
		const char* _name
	) const;

public:

	const Shader& GetTextShader() const { return m_TextRenderingShader; }
	const Font& GetDefaultFont() const { return m_DefaultFont; }
	
	Batch* GetUIBatch() { return &m_UIBatch; }

public:

	const char* c_SpecialTextShaderName = "SPECIAL_COMMON_TEXT_SHADER";
	const char* c_SpecialUISheetName	= "SPECIAL_UI_SPRITESHEET_NAME";

};