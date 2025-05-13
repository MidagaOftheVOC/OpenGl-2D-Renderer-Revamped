#pragma once

#include <vector>
#include <queue>

#include "common/common.h"

#include "components/sprite_sheet.h"
#include "components/shader.h"
#include "components/drawable.h"
#include "components/batch_types/batch.h"
#include "components/camera.h"

#include "components/uniform/uniform_data.h"

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
private:	//	Window-related information

	int m_ScreenWidth = -1;
	int m_ScreenHeight = -1;
	std::string m_WindowTitle = nullptr;

	bool m_Fullscreen = false;

	bool m_IsRunning = false;
	GLFWwindow* m_MainWindowHandle = nullptr;

private:	//	Logical components

	StandardQuad m_StandardQuad;

	Camera m_Camera;

	std::vector<SpriteSheet> m_SpriteSheetArray;
	std::vector<Shader> m_ShaderArray;


private:	//	Structures for draw queue optimisation

	//	Drawable queue
	struct DrawCall {
		const Drawable* drawable = nullptr;
		float xScreenCoord, yScreenCoord;
		float zLayer = 2.f;

		//	This is really expensive, should probably only be used 
		const UniformDataVector* m_AppliedUniforms = nullptr;

		DrawCall(const Drawable* _drawable, float x, float y, float z, const UniformDataVector* _uniformDataArray)
			: drawable(_drawable), xScreenCoord(x), yScreenCoord(y), zLayer(z), m_AppliedUniforms(_uniformDataArray)
		{}

		glm::vec3 GetPositionVector() { return glm::vec3(xScreenCoord, yScreenCoord, zLayer); }
	};

	//	This comparator orders DrawCalls first by Shader*, and within
	//	those Shader* groups, second by SpriteSheet*
	struct DrawCallComparator{
		bool operator()(const DrawCall& a, const DrawCall& b) const;
	};


	std::priority_queue<DrawCall, std::vector<DrawCall>, DrawCallComparator> m_DrawCallQueue;

	
	//	Batch queue
	



public:		//	Exposed functions
	
	Renderer2D(
		int _screenWidth,
		int _screenHeight,
		const char* _windowTitle,
		bool _fullscreen = false
	);

	
	//	Initialise window and global variables.
	//	Returns false on fail.
	bool Init();


	//	Execute all draw calls and flush the draw queue.
	void ExecuteDraws();


	//	Adds a draw call to the queue for _drawable at position {_xPosition, _yPosition} with
	//	top left window corner as origin, with y-axis growing downwards.
	void Draw(
		const Drawable* _drawable,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	//	Returns a brand new Drawable object derived from an existing SpriteSheet object.
	// 
	//	!!! This, used anywhere else other than a loading process that exposes
	//	all Drawable instances from each SpriteSheet, violates the Drawable doctrine
	//	of being a one-for-all templates.
	Drawable GenerateDrawable(
		const char* _spriteSheetName,
		int _indexInSpriteSheet
	);

public:

	void ExtractDrawablesFromSheets(
		std::vector<Drawable>& OUT_drawableVector
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

public:		// getters and setters, 

	Shader* GetShaderByName(
		const char* _shaderName
	);


	SpriteSheet* GetSpriteSheetByName(
		const char* _spriteSheetName
	);

	//	Gets the single instance governing OpenGL objects like 
	//	VBOs, IBO and VAO used for rendering
	StandardQuad&	GetQuad() { return m_StandardQuad; }
	GLFWwindow*		GetWinHandle() { return m_MainWindowHandle; }
};



