#pragma once

#include <vector>
#include <queue>

#include "common/common.h"
#include "common/standard_quad.h"

#include "components/sprite_sheet.h"
#include "components/shader.h"
#include "components/drawable.h"
#include "components/camera.h"

#include "components/batch_types/strict_batch.h"

#include "components/uniform/uniform_data.h"




class Renderer2D {
private:	//	Window-related information

	int m_ScreenWidth = -1;
	int m_ScreenHeight = -1;
	std::string m_WindowTitle = nullptr;

	bool m_Fullscreen = false;

	bool m_IsRunning = false;
	GLFWwindow* m_MainWindowHandle = nullptr;

private:	//	Logical components

	StandardQuad& m_StandardQuad;

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

		glm::vec3 GetPositionVector() const { return glm::vec3(xScreenCoord, yScreenCoord, zLayer); }
	};

	//	This comparator orders DrawCalls first by Shader*, and within
	//	those Shader* groups, second by SpriteSheet*
	struct DrawCallComparator{
		bool operator()(const DrawCall& a, const DrawCall& b) const;
	};


	std::priority_queue<DrawCall, std::vector<DrawCall>, DrawCallComparator> m_DrawCallQueue;

	
	//	Batch array
	//	This doesn't need to be an array
	struct StrictBatchDrawCall {
		const StrictBatch* m_Strict = nullptr;
		float m_xScreenCoord, m_yScreenCoord;
		float m_zLayer;

		const UniformDataVector* m_AppliedUniforms = nullptr;

		StrictBatchDrawCall(const StrictBatch* _strictBatch, float x, float y, float z, const UniformDataVector* _uniformDataArray)
			: m_Strict(_strictBatch), m_xScreenCoord(x), m_yScreenCoord(y), m_zLayer(z), m_AppliedUniforms(_uniformDataArray)
		{}

		glm::vec3 GetPositionVector() const { return glm::vec3(m_xScreenCoord, m_yScreenCoord, m_zLayer); }
	};

	std::vector<StrictBatchDrawCall> m_StrictBatchArray;

private:

	void RenderDrawables();


	void RenderStrictBatches();

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


	void Draw(
		const StrictBatch* _batch,
		float _initialXpos,
		float _initialYpos,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


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
	Camera&			GetCamera() { return m_Camera; }
};



