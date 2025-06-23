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
#include "components/batch_types/free_batch.h"
#include "components/batch_types/ui_batch.h"

#include "components/uniform/uniform_data.h"

#include "components/ui/ui_manager.h"
#include "components/ui/text.h"



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


	UIManager m_UIManager;


	std::vector<SpriteSheet> m_SpriteSheetArray;
	std::vector<Shader> m_ShaderArray;

private:

	Shader m_TextRenderingShader;

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

	//		DRAWABLE

	struct DrawableDrawCall : DrawCall {
	private:
		const Drawable* m_Drawable = nullptr;
	public:
		DrawableDrawCall(const Drawable* _drawable, float x, float y, float z, const UniformDataVector* _uniformDataArray)
			: m_Drawable(_drawable), DrawCall(x, y, z, _uniformDataArray)
		{}
		const Drawable* GetDrawablePointer() const { return m_Drawable; }
	};

	struct DrawCallComparator{
		bool operator()(const DrawableDrawCall& a, const DrawableDrawCall& b) const;
	};

	std::priority_queue<DrawableDrawCall, std::vector<DrawableDrawCall>, DrawCallComparator> m_DrawCallQueue;

	//		BATCHES
	
	struct BatchDrawCall: DrawCall {
	private:
		const BaseBatch* m_Base = nullptr;
	public:
		BatchDrawCall(const BaseBatch* _softBatch, float x, float y, float z, const UniformDataVector* _uniformDataArray)
			: m_Base(_softBatch), DrawCall(x, y, z, _uniformDataArray)
		{}
		//	RECAST AS APPROPRIATE
		const BaseBatch* GetBaseBatchPointer() const { return m_Base; }
	};

	std::vector<BatchDrawCall> m_StrictBatchArray;
	std::vector<BatchDrawCall> m_SoftBatchArray;
	std::vector<BatchDrawCall> m_FreeBatchArray;

	//		TEXT

	struct TextDrawCall : DrawCall {
	private:
		const Text* m_Text = nullptr;
	public:
		TextDrawCall(const Text* _text, float x, float y, float z, const UniformDataVector* _uniformDataArray)
			:m_Text(_text), DrawCall(x, y, z, _uniformDataArray)
		{}
		const Text* GetTextPointer() const { return m_Text; }
	};

	std::vector<TextDrawCall> m_TextArray;

	//		UI

	struct UIDrawCall : DrawCall {
	private:
		const UIBatch* m_UIBatch = nullptr;
	public:	//	Point is that UI elements will always have the relevant positions in the batch itself
			//	since those batches are supposed to be used en-masse, i.e. one such batch for each primitive
			//	That is to say, all Panes, or all Buttons for rendering, will be done so with one batch for each.
		UIDrawCall(const UIBatch* _uiBatch, float _cameraXpos, float _cameraYpos,float _furthestZcoord,const UniformDataVector* _uniformDataArray)
			:m_UIBatch(_uiBatch), DrawCall(_cameraXpos, _cameraYpos, _furthestZcoord, _uniformDataArray)
		{}
		const UIBatch* GetUIBatchPointer() const { return m_UIBatch; }
	};

	std::vector<UIDrawCall> m_UIBatchArray;

private:	//	Methods to render queued Drawcalls

	void RenderText();


	void RenderDrawables();


	void RenderStrictBatches();


	void RenderSoftBatches();


	void RenderFreeBatches();


	void RenderGUI();

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


	//	Execute all draw calls.
	void ExecuteDraws();

	//	Here, the X and Y coordinates represent the anchor point for the SoftBatch.
	//	In essence, all the positions given to the SoftBatch object at creation
	//	are relative to the X and Y coordinates given here.
	void Draw(
		const SoftBatch* _batch,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		const FreeBatch* _batch,
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
		UniformDataVector* _uniformArray
	);


	void Draw(
		const Drawable* _drawable,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		const Text* _text,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		const UIBatch* _uiBatch,
		float _furthestZcoord,
		UniformDataVector* _uniformArray
	);


	//	Returns a brand new Drawable object derived from an existing SpriteSheet object.
	// 
	//	!!! This, used anywhere else other than a loading process that exposes
	//	all Drawable instances from each SpriteSheet, violates the Drawable doctrine
	//	of being a one-for-all templates.
	//
	//	Note: To be frank, this is pretty shit. You're better off pooling things in 
	//	soft batches with floating quads.
	Drawable GenerateDrawable(
		const char* _spriteSheetName,
		int _indexInSpriteSheet
	);


	bool IsRunning() const;

public:		// Exposed constants

	const char* c_SpecialTextShaderName = "SPECIAL_COMMON_TEXT_SHADER";

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

private:	//	Following functions executer the load params for shaders and
			//	SpriteSheets and properly linking them.

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

private:

	void PerClassVAOinitialisationFunction();

public:		// getters and setters, 

	const Shader* GetShaderByName(
		const char* _shaderName
	);


	const SpriteSheet* GetSpriteSheetByName(
		const char* _spriteSheetName
	);

	//	Gets the single instance governing OpenGL objects like 
	//	VBOs, IBO and VAO used for rendering
	StandardQuad&		GetQuad() { return m_StandardQuad; }
	GLFWwindow*			GetWinHandle() const { return m_MainWindowHandle; }
	Camera&				GetCamera() { return m_Camera; }
	InputController&	GetInputController() { return m_InputController; }

	Shader&				GetTextShader() { return m_TextRenderingShader; }

};



