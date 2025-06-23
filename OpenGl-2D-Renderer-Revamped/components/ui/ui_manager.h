#pragma once


#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include "../batch_types/free_batch.h"


#include "window.h"


class Renderer2D;


struct PaneSupplementDataStruct {
	float m_Rotations[9] = { 0.f };
	SpriteInformation m_SpriteInformation[9];
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


	Renderer2D* m_MainRenderer = nullptr;

private:
	


private:

	std::vector<Window> m_WindowArray;

public:

	UIManager(
		Renderer2D* _renderer
	);


	//	Iterate over the Window objects.
	void RenderAllActiveWindows(

	);


	//	Adds window to the GUI
	void AddWindow(
		Window _window
	);
	

	void RemoveWindow(
		ID _windowID
	);

};
