#pragma once

#include <vector>

#include "common/common.h"

#include "components/sprite_sheet.h"
#include "components/shader.h"
#include "components/drawable.h"
#include "components/batch.h"


class Renderer2D {

	// TODO:	marked for removal
	// Will be used once at init to build up the SpriteRegionArray
	std::vector<SpriteSheet> m_SpriteSheetArray; 
	std::vector<Shader> m_ShaderArray;
	std::vector<SpriteRegion> m_SpriteRegionArray;

	GLFWwindow* m_MainWindowHandle = nullptr;

public:
	
	Renderer2D() {}

	void Init();
	
	
	void Draw(
		Drawable _drawable,
		float _xPosition,
		float _yPosition
	);

	
	



	glm::vec2 GetMousePosition();


public:

	SpriteSheet* GetSpriteSheetByName(
		const char* _spriteSheetName
	);


	Shader* GetShaderByName(
		const char* _shaderName
	);


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
};



