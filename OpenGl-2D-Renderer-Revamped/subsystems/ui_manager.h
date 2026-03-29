#pragma once


#include "../../subsystems/main_renderer.h"
#include "../../subsystems/resource_service.h"

#include "../components/input_controller.h"

#include "../components/batch_types/ui_batch.h"

#include "factories/pane_factory.h"
#include "../components/ui/primitives/types/label.h"
#include "../components/ui/primitives/types/button.h"

#include "../components/ui/window.h"



class UIManager {

	//	From the renderer-assigned usable Z space
	//	This here tells us where we can and cannot draw UI elements

	float m_LowerBoundOfZlayerDistribution = 0.f;
	float m_UpperBoundOfZlayerDistribution = 0.f;

	//	Layers of panes will be stored as ints, final Z value
	//	taken by LayerIndex * (this float value)
	float m_CalculatedWindowZlayerStep = 0.f;
	float m_CalculatedPrimitiveZlayerStep = 0.f;
	/*
	
	Suppose each window has its own distinct layers, 1 for the pane and 3 for primitives

	So we get a step of  (Upper - Lower) / (WindowCount * 4)
	
	*/

	ID m_UniqueWindowIDCounter = 0;

	const Renderer2D* m_Renderer = nullptr;
	const InputController* m_Input = nullptr;
	const ResourceService* m_ResService = nullptr;

private:	//	Windows
	
	std::vector<Window> m_WindowArray;

	std::vector<ID> m_WindowIDArrayForRendering;

	std::vector<TextWithZLayer> m_CachedTexts;

private:	//	Factories for UI primitives


private:	//	Batches

	UIBatch* m_UIBatch = nullptr;
	

	void UpdateAllBatches();

public:		//	Constants

	const size_t c_ForemostWindowLayer = 0;

public:

	UIManager() {}

	UIManager(UIBatch* _uiBatch);

	void SetUiBatchPointer(UIBatch* _uiBatch);

	//	If nullptr is passed as skin name, the window won't have a background
	Window CreateWindow(
		glm::vec2 _windowDimensions,
		float _cornerLengthPx,
		const char* _uiSkinName = "default"
	);

	//	Adds window to the GUI
	ID AddWindow(
		Window _window
	);

	void RemoveWindow(
		ID _windowID
	);

	void OpenWindow(
		ID _windowID,
		float _xCoord,
		float _yCoord
	);

	void CloseWindow(
		ID _windowID
	);

	void MoveWindowToLayer(
		ID _windowID,
		size_t _layer
	);

	void MoveWindowToFront(
		ID _windowID
	);
	 
private:
	
	size_t GetIndexOfWindowByIDInRenderArray(
		const ID _windowID
	) const;


	ID GetNextUniqueWindowID();


	size_t CountAllWindowsWithBackgrounds() const;


	Window* GetWindowByID(
		const ID _windowID
	);


	Window* GetWindowByIDFromRenderArray(
		const ID _windowID
	);


	bool IsWindowIDInRenderArray(
		const ID _windowID
	) const;

public:

	//	Returns 0 on no overlaps
	const ID HasClickedOnUIElement(
		float xMousePos,
		float yMousePos
	) const;

	void ProvokeUIActionWithMouseCoords(
		const InputController* _input,
		ID _winId
	);

	void SetDistributionBounds(float _low, float _high);

	const UIBatch* GetUIBatch() { return m_UIBatch; }
	const std::vector<TextWithZLayer>& GetUITexts() const { return m_CachedTexts; }

	const std::vector<Window>& GetWindows() const { return m_WindowArray; }

	const Renderer2D* GetRenderer() const { return m_Renderer; }
	const InputController* GetInput() const { return m_Input; }
	const ResourceService* GetResources() const { return m_ResService; }

	void SetRenderer(const Renderer2D* rend) { m_Renderer = rend; }
	void SetInputController(const InputController* input) { m_Input = input; }
	void SetResService(const ResourceService* res) { m_ResService = res; }
};
