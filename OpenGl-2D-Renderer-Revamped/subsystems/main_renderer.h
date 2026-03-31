#pragma once

#include <vector>
#include <queue>

#include "resource_service.h"

#include "../common/common.h"
#include "../common/standard_quad.h"
#include "../components/sprite_sheet.h"
#include "../components/shader.h"
#include "../components/drawable.h"
#include "../components/camera.h"
#include "../components/input_controller.h"
#include "../components/file_handler.h"

#include "../components/batch_types/strict_batch.h"
#include "../components/batch_types/soft_batch.h"
#include "../components/batch_types/free_batch.h"
#include "../components/batch_types/ui_batch.h"
#include "../components/uniform/uniform_data.h"

#include "../components/ui/text.h"

struct GameLoopReturnType {
	FreeBatch* batches = nullptr;
	int count = 0;
};

//	Base class for DrawCall-like structures
//	This contains required data for all draw calls like positions and a uniform array
//	This class should not(and cannot as of now) be used anywhere else, one should only
//	derive from it to create specialised draw calls.
struct DrawCall {
protected:
	float m_xScreenCoord;
	float m_yScreenCoord;
	float m_zLayer;
	size_t m_MaxInstanceCapacity = 0;

	//	If value is left at nullptr, no additional uniforms will be applied
	const UniformDataVector* m_AppliedUniforms = nullptr;

public:
	DrawCall(float x, float y, float z, const UniformDataVector* _uniformDataArray, size_t _instances)
		: m_xScreenCoord(x), m_yScreenCoord(y), m_zLayer(z), m_AppliedUniforms(_uniformDataArray), m_MaxInstanceCapacity(_instances)
	{}

	const UniformDataVector* GetAppliedUniforms() const { return m_AppliedUniforms; }
	glm::vec3 GetPositionVector() const { return glm::vec3(m_xScreenCoord, m_yScreenCoord, m_zLayer); }
	const size_t GetInstances() const { return m_MaxInstanceCapacity; }
};

//		DRAWABLE

struct DrawableDrawCall : DrawCall {
private:
	const Drawable* m_Drawable = nullptr;
public:
	DrawableDrawCall(const Drawable* _drawable, float x, float y, float z, const UniformDataVector* _uniformDataArray, size_t _instances)
		: m_Drawable(_drawable), DrawCall(x, y, z, _uniformDataArray, _instances)
	{
	}
	const Drawable* GetDrawablePointer() const { return m_Drawable; }
};

struct DrawCallComparator {
	bool operator()(const DrawableDrawCall& a, const DrawableDrawCall& b) const;
};

//		BATCHES

struct BatchDrawCall : DrawCall {
private:
	const BaseBatch* m_Base = nullptr;
public:
	BatchDrawCall(const BaseBatch* _softBatch, float x, float y, float z, const UniformDataVector* _uniformDataArray, size_t _instances)
		: m_Base(_softBatch), DrawCall(x, y, z, _uniformDataArray, _instances)
	{
	}
	//	RECAST AS APPROPRIATE
	const BaseBatch* GetBaseBatchPointer() const { return m_Base; }
};

//		TEXT

struct TextDrawCall : DrawCall {
private:
	const Text* m_Text = nullptr;
public:
	TextDrawCall(const Text* _text, float x, float y, float z, const UniformDataVector* _uniformDataArray)
		:m_Text(_text), DrawCall(x, y, z, _uniformDataArray, 0)
	{
	}
	const Text* GetTextPointer() const { return m_Text; }
};

//		UI

struct UIDrawCall : DrawCall {
private:
	const UIBatch* m_UIBatch = nullptr;
public:	//	Point is that UI elements will always have the relevant positions in the batch itself
	//	since those batches are supposed to be used en-masse, i.e. one such batch for each primitive
	//	That is to say, all Panes, or all Buttons for rendering, will be done so with one batch for each.
	UIDrawCall(const UIBatch* _uiBatch, float _cameraXpos, float _cameraYpos, float _furthestZcoord, const UniformDataVector* _uniformDataArray, size_t _instances)
		:m_UIBatch(_uiBatch), DrawCall(_cameraXpos, _cameraYpos, _furthestZcoord, _uniformDataArray, _instances)
	{
	}
	const UIBatch* GetUIBatchPointer() const { return m_UIBatch; }
};

class Renderer2D {
private:	//	Window-related information

	int m_ScreenWidth = -1;
	int m_ScreenHeight = -1;
	std::string m_WindowTitle = "";

	bool m_Fullscreen = false;

	bool m_IsRunning = false;
	GLFWwindow* m_MainWindowHandle = nullptr;

	const InputController* m_InputController = nullptr;
	const ResourceService* m_ResService = nullptr;

private:	//	Logical components

	StandardQuad m_StandardQuad;

	Camera m_Camera;

private:

	bool m_HasClickedThisFrame = false;

public:

	const bool HasClicked();

private:	//	Structures for draw queue optimisation

	std::priority_queue<DrawableDrawCall, std::vector<DrawableDrawCall>, DrawCallComparator> m_DrawCallQueue;
	std::vector<BatchDrawCall> m_StrictBatchArray;
	std::vector<BatchDrawCall> m_SoftBatchArray;
	std::vector<BatchDrawCall> m_FreeBatchArray;
	std::vector<TextDrawCall> m_TextArray;
	std::vector<UIDrawCall> m_UIBatchArray;

private:	//	Methods to render queued Drawcalls

	void RenderText();


	void RenderDrawables();


	void RenderStrictBatches();


	void RenderSoftBatches();


	void RenderFreeBatches();


	void RenderGUI();

public:		//	Exposed functions

	Renderer2D() {}

	
	Renderer2D(
		GLFWwindow* _initialisedWindow,
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
		SoftBatch* _batch,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		FreeBatch* _batch,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		StrictBatch* _batch,
		float _initialXpos,
		float _initialYpos,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		Drawable* _drawable,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		Text* _text,
		float _xPosition,
		float _yPosition,
		float _zLayer,
		UniformDataVector* _uniformArray
	);


	void Draw(
		UIBatch* _uiBatch,
		float _furthestZcoord,
		UniformDataVector* _uniformArray
	);

private:

	void PerClassVAOinitialisationFunction();

public:

	void SendStandardUniforms(
		const Shader* _targetShader,
		const glm::vec3& _positionVector
	) const;

	//	Gets the single instance governing OpenGL objects like 
	//	VBOs, IBO and VAO used for rendering
	GLFWwindow*			GetWinHandle() const { return m_MainWindowHandle; }	// always required to be non-const
	Camera&				GetCamera() { return m_Camera; }


	const Camera&			GetCamera() const { return m_Camera; }


	const InputController* GetInput() const { return m_InputController; }
	const ResourceService* GetResources() const { return m_ResService; }

	void SetInputController(const InputController* input) { m_InputController = input; }
	void SetResService(const ResourceService* res) { m_ResService = res; }

	StandardQuad& GetQuad() { return m_StandardQuad; }
};