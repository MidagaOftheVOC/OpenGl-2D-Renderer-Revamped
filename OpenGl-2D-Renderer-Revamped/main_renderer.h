#pragma once

#include <vector>
#include <queue>

#include "common/common.h"
#include "common/standard_quad.h"

#include "components/sprite_sheet.h"
#include "components/shader.h"
#include "components/drawable.h"
#include "components/camera.h"
#include "components/input_controller.h"

#include "components/batch_types/strict_batch.h"
#include "components/batch_types/soft_batch.h"

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


	InputController m_InputController;


	std::vector<SpriteSheet> m_SpriteSheetArray;
	std::vector<Shader> m_ShaderArray;


private:	//	Structures for draw queue optimisation

	//	Base class for DrawCall-like structures
	//	This contains required data for all draw calls like positions and a uniform array
	//	This class should not(and cannot as of now) be used anywhere else, one should only
	//	derive from it to create specialised draw calls.
	struct DrawCall {
	protected:
		float m_xScreenCoord;
		float m_yScreenCoord;
		float m_zLayer;

		//	If value is left at nullptr, no additional uniforms will be applied
		const UniformDataVector* m_AppliedUniforms = nullptr; 

	public:
		DrawCall(float x, float y, float z, const UniformDataVector* _uniformDataArray)
			: m_xScreenCoord(x), m_yScreenCoord(y), m_zLayer(z), m_AppliedUniforms(_uniformDataArray)
		{}

		const UniformDataVector* GetAppliedUniforms() const { return m_AppliedUniforms; }
		glm::vec3 GetPositionVector() const { return glm::vec3(m_xScreenCoord, m_yScreenCoord, m_zLayer); }
	};


	struct DrawableDrawCall : DrawCall {
	private:
		const Drawable* m_Drawable = nullptr;
	public:
		DrawableDrawCall(const Drawable* _drawable, float x, float y, float z, const UniformDataVector* _uniformDataArray)
			: m_Drawable(_drawable), DrawCall(x, y, z, _uniformDataArray)
		{}
		const Drawable* GetDrawablePointer() const { return m_Drawable; }
	};


	//	This comparator orders DrawCalls first by Shader*, and within
	//	those Shader* groups, second by SpriteSheet*
	struct DrawCallComparator{
		bool operator()(const DrawableDrawCall& a, const DrawableDrawCall& b) const;
	};


	std::priority_queue<DrawableDrawCall, std::vector<DrawableDrawCall>, DrawCallComparator> m_DrawCallQueue;

	
	struct StrictBatchDrawCall : DrawCall {
	private:
		const StrictBatch* m_Strict = nullptr;
		const int m_RowSpriteCount;
	public:
		StrictBatchDrawCall(const StrictBatch* _strictBatch, float x, float y, float z, const UniformDataVector* _uniformDataArray, int _rowCount)
			: m_Strict(_strictBatch), m_RowSpriteCount(_rowCount), DrawCall(x, y, z, _uniformDataArray)
		{}
		const StrictBatch* GetStrictBatchPointer() const { return m_Strict; }
		const int GetRowSpriteCount() const { return m_RowSpriteCount; }
	};

	
	std::vector<StrictBatchDrawCall> m_StrictBatchArray;


	struct SoftBatchDrawCall : DrawCall {
	private:
		const SoftBatch* m_Soft = nullptr;
	public:
		SoftBatchDrawCall(const SoftBatch* _softBatch, float x, float y, float z, const UniformDataVector* _uniformDataArray)
			: m_Soft(_softBatch), DrawCall(x, y, z, _uniformDataArray)
		{}
		const SoftBatch* GetSoftBatchPointer() const { return m_Soft; }
	};


	std::vector<SoftBatchDrawCall> m_SoftBatchArray;


private:

	void RenderDrawables();


	void RenderStrictBatches();


	void RenderSoftBatches();

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
		const SoftBatch* _batch,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		const StrictBatch* _batch,
		float _initialXpos,
		float _initialYpos,
		float _zLayer,
		int _rowSpriteCount,
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


	bool IsRunning() const;

public:		//	Loading functions, which append loading parameters to the queues.
			//	All necessary resources must pass through those, after which we
			//	call StartLoadingProcess() to turn them into usable objects.
			//
			//	Note:	At calling StartLoadingProcess(), all necessary data must 
			//	be in the queues, even previously allocated, if you'll be switching scenes
			//	or similar.

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

private:	//	The loading functions in this block begin the actual OpenGL
			//	initialisation process, taking data from the load params queues.

	struct ShaderLoadingParameters {
		const std::string m_ShaderName;
		const std::string m_LocationOfShaderFile;
		ShaderLoadingParameters(
			const char* _location,
			const char* _shaderName
		)
			: m_ShaderName(_shaderName), m_LocationOfShaderFile(_location) {}
	};

	std::vector<ShaderLoadingParameters> m_ShaderLoadQueue;

	void LoadShader(
		const std::string& _locationShaderFile,
		const std::string& _shaderName
	);


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

	std::vector<SpriteSheetLoadingParameters> m_SpriteSheetLoadQueue;

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
	StandardQuad&		GetQuad() { return m_StandardQuad; }
	GLFWwindow*			GetWinHandle() const { return m_MainWindowHandle; }
	Camera&				GetCamera() { return m_Camera; }
	InputController&	GetInputController() { return m_InputController; }

};



