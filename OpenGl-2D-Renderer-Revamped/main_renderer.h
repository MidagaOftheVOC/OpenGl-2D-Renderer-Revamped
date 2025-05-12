#pragma once

#include <vector>
#include <queue>

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

	UVRegion m_UVregionCurrentlyUsed;
	const UVRegion& GetCurrentUVregion() const { return m_UVregionCurrentlyUsed; }


	void Init();
	void Bind();
	void Unbind();

	//	Returns true if this and last used SpriteSheet are the same
	//	and no buffering happens.
	//	Returns false if they're different and buffering does occur.
	bool BufferTexCoords(
		const SpriteSheet* _spriteSheet
	);


public:

	unsigned int  g_stdVertexCoordArray[8] = {
		0,										0,
		0,										(unsigned)m_StandardSpritePixelLength,
		(unsigned)m_StandardSpritePixelLength,	(unsigned)m_StandardSpritePixelLength,
		(unsigned)m_StandardSpritePixelLength,	0
	};

	float g_stdTexCoordArray[8] = {
		0.f,	0.f,
		0.f,	1.f,
		1.f,	1.f,
		1.f,	0.f
	};

	unsigned short g_stdIndexArray[6] = {
		0,	1,	2,
		0,	2,	3
	};

};

class Renderer2D {

	
	int m_ScreenWidth = -1;
	int m_ScreenHeight = -1;
	std::string m_WindowTitle = nullptr;

	bool m_Fullscreen = false;
	GLFWwindow* m_MainWindowHandle = nullptr;

private:

	StandardQuad m_StandardQuad;

	Camera m_Camera;

	std::vector<SpriteSheet> m_SpriteSheetArray;
	std::vector<Shader> m_ShaderArray;


private:

	struct DrawCall {
		const Drawable* drawable;
		float xScreenCoord, yScreenCoord;
		float zLayer = 2.f;
		DrawCall(const Drawable* _drawable, float x, float y, float z)
			: drawable(_drawable), xScreenCoord(x), yScreenCoord(y), zLayer(z) {}
		glm::vec3 GetPositionVector() { return glm::vec3(xScreenCoord, yScreenCoord, zLayer); }
	};

	struct DrawCallComparator{
		bool operator()(const DrawCall& a, const DrawCall& b) const {
			return a.drawable->GetAsociatedSpriteSheet() > b.drawable->GetAsociatedSpriteSheet();
		}
	};

	std::priority_queue<DrawCall, std::vector<DrawCall>, DrawCallComparator> m_DrawCallQueue;

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


	void ExecuteDraws();


	void Draw(
		const Drawable* _drawable,
		float _xPosition,
		float _yPosition,
		float _zLayer
	);



public:

	//	Returns a brand new Drawable object derived from an existing SpriteSheet object.
	//	This, used anywhere else other than the loading process that exposes
	//	all Drawable instances for each SpriteSheet, violates the Drawable doctrine
	//	of being a one-for-all template.
	Drawable GetDrawable(
		const char* _spriteSheetName,
		int _indexInSpriteSheet
	);


	GLFWwindow* GetWinHandle() { return m_MainWindowHandle; }

public:

	//	Gets the single instance governing OpenGL objects like 
	//	VBOs, IBO and VAO used for rendering
	StandardQuad& GetQuad() { return m_StandardQuad; }


	void ExtractDrawablesFromSheets(
		std::vector<Drawable> OUT_drawableVector
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

	SpriteSheet* GetSpriteSheetByName(
		const char* _spriteSheetName
	);
};



