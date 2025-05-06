#pragma once

#include <vector>

#include "common/common.h"

#include "components/sprite_sheet.h"
#include "components/shader.h"
#include "components/drawable.h"
#include "components/batch.h"
#include "components/camera.h"

struct StandardQuad {

	int m_StandardSpritePixelLength = 100;

	unsigned int m_VAO = 0;

	unsigned int m_VextexBuffer = 0;
	unsigned int m_TexCoordBuffer = 0;
	unsigned int m_IndexBuffer = 0;

	void Init();
	void Bind();
	void Unbind();

	void BufferTexCoords(
		const SpriteRegion* _spriteRegion
	);

};

class Renderer2D {

	StandardQuad m_StandardQuad;

	Camera m_Camera;

	std::vector<SpriteSheet> m_SpriteSheetArray;	// TODO: marked for removal
	std::vector<Shader> m_ShaderArray;
	std::vector<SpriteRegion> m_SpriteRegionArray;


	GLFWwindow* m_MainWindowHandle = nullptr;

	
	int m_ScreenWidth = -1;
	int m_ScreenHeight = -1;
	std::string m_WindowTitle = nullptr;

	bool m_Fullscreen = false;

public:
	
	Renderer2D(
		int _screenWidth,
		int _screenHeight,
		const char* _windowTitle,
		bool _fullscreen = false
	);

	
	//	Initialise window and global variables.
	//	Returns false on fail.
	bool Init();


	void Draw(
		Drawable _drawable,
		int _xPosition,
		int _yPosition
	);




public:

	SpriteRegion* GetSpriteRegionByIndex(int index) { return &m_SpriteRegionArray[index]; }

	StandardQuad& GetQuad() { return m_StandardQuad; }

	GLFWwindow* GetWinHandle() { return m_MainWindowHandle; }

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



