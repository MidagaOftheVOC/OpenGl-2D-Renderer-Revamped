#pragma once


#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include "../batch_types/ui_batch.h"


#include "primitives/factories/pane_factory.h"
#include "primitives/types/label.h"

#include "window.h"

struct TextWithZLayer {
	TextWithZLayer(
		const Text* _text,
		float _x,
		float _y,
		float _zLayer
	):
		text(_text),
		xLayer(_x),
		yLayer(_y),
		zLayer(_zLayer)
	{}
	const float xLayer = 0.f;
	const float yLayer = 0.f;
	const float zLayer = 0.f;
	const Text* text = nullptr;
};

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

private:	//	Windows
	
	std::vector<Window> m_WindowArray;

	std::vector<ID> m_WindowIDArrayForRendering;

	std::vector<TextWithZLayer> m_CachedTexts;

private:	//	Factories for UI primitives

	PaneFactory m_ThePaneFactory;

private:	//	Batches

	UIBatch* m_UIBatch = nullptr;
	

	void UpdateAllBatches();

public:		//	Constants

	const size_t c_ForemostWindowLayer = 0;

public:

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

	void SetDistributionBounds(float _low, float _high);

	const UIBatch* GetUIBatch() { return m_UIBatch; }
	PaneFactory& GetPaneFactory() { return m_ThePaneFactory; }
	const std::vector<TextWithZLayer>& GetUITexts() const { return m_CachedTexts; }

};
