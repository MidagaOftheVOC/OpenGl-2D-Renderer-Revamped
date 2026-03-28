#pragma once

#include "../components/shader.h"
#include "../components/sprite_sheet.h"
#include "../components/ui/font.h"
#include "../components/ui/primitives/factories/pane_factory.h"

class ResourceService {

	GLFWwindow* m_MainWindowContext = nullptr;

private:

	std::vector<Shader> m_Shaders;
	std::vector<SpriteSheet> m_Sheets;

	std::vector<Font> m_Fonts;
	std::vector<PaneSkin> m_PaneSkins;


	std::vector<UploadShaderParameters> m_ShaderLoadQueue;
	std::vector<UploadSpriteSheetParameters> m_SpriteSheetLoadQueue;

private:

	Shader m_TextRenderingShader;

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

	const Shader* GetShaderByName(
		const char* _shaderName
	);

	const SpriteSheet* GetSpriteSheetByName(
		const char* _spriteSheetName
	);

public:

private:

	const char* c_SpecialTextShaderName = "SPECIAL_COMMON_TEXT_SHADER";
	const char* c_SpecialUISheetName = "SPECIAL_UI_SPRITESHEET_NAME";

};