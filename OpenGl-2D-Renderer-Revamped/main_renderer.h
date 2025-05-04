#pragma once

#include <vector>

#include "common/common.h"

#include "components/sprite_sheet.h"
#include "components/shader.h"

class Renderer2D {

	// TODO:	marked for removal
	// Will be used once at init to build up the SpriteRegionArray
	std::vector<SpriteSheet> m_SpriteSheetArray; 
	std::vector<Shader> m_ShaderArray;
	std::vector<SpriteRegion> m_SpriteRegionArray;

public:
	
	Renderer2D() {}

	void Init();
	
	



	void DrawAt(
		Drawable _drawable,
		int _xCoord,
		int _yCoord,
		const char* _shaderName,
		bool true_if_ignore_forced_dims
	);


	void WipeRenderer();

	

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
		int _spritesPerRow,
		int _spritesPerCol
	);
};



