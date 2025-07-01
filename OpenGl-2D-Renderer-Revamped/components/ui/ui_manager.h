#pragma once


#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include "../batch_types/ui_batch.h"


#include "primitives/factories/pane_factory.h"


#include "window.h"


class Renderer2D;


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

	Renderer2D* m_MainRenderer = nullptr;


	ID m_UniqueWindowIDCounter = 0;

private:
	
	std::vector<Window> m_WindowArray;
	

	std::vector<ID> m_WindowIDArrayForRendering;

private:	//	Factories for UI primitives

	PaneFactory m_ThePaneFactory;

private:

	UIBatch m_PaneBatch = UIBatch(9);
	

	void UpdateAllBatches();

public:

	UIManager(
		Renderer2D* _renderer
	);


	//	Iterate over the Window objects.
	void RenderAllActiveWindows();


	//	If nullptr is passed as skin name, the window won't have a background
	Window CreateWindow(
		glm::vec2 _windowDimensions,
		float _cornerLengthPx,
		const char* _uiSkinName
	);


	//	Adds window to the GUI
	ID AddWindow(
		Window _window
	);
	

	void RemoveWindow(
		ID _windowID
	);


	//	The placed window will automatically be put at the forefront.
	void OpenWindow(
		ID _windowID,
		float _xCoord,
		float _yCoord
	);


	void CloseWindow(
		ID _windowID
	);


	//	Layer 0 is the foremost layer.
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

	UIBatch& GetPaneBatch() { return m_PaneBatch; }
	PaneFactory& GetPaneFactory() { return m_ThePaneFactory; }

};
