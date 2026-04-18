#include "ui_manager.h"

void UIManager::SetZSpaceDistribution(
	float zClose,
	float zFurther
) {
	m_CloserBoundOfZLayerDistribution = zClose;
	m_FurtherBoundOfZLayerDistribution = zFurther;
}

void UIManager::InterpretInput() {
	GameInput& input = GetInput()->ExposeGameInput();

	/* process keyboard here */

	Batch* uiBatch = GetResService()->GetUIBatch();
	Batch* textBatch = GetResService()->GetUITextBatch();

	const int MAXIMUM_NESTED_WIDGETS = 4;
	const int totalLayers = 1 + m_OpenedWindows.size();	//	dont forget UI-manager ui
	const int totalSublayers = totalLayers * MAXIMUM_NESTED_WIDGETS;
	
	
	const float baseZLayer = m_FurtherBoundOfZLayerDistribution;
	const float workRange = GetFurtherZBound() - GetCloserZBound();
	const float substep = - (workRange / float(totalSublayers));

	
	//	Window in the END of the vector is closest to us.
	//	We render from bottom to top
	//	We detect from top to bottom

	//	---		RENDER		---	 //

	//	UI manager
	//	TODO: replace this with a special background-less window
	RenderWidgetTree(uiBatch, textBatch, glm::vec2(0.f, 0.f), baseZLayer, substep);

	//	render windows
	for (size_t i = 0; i < m_OpenedWindows.size(); i++) {
		float correctBaseZlayer = baseZLayer + ((i + 1) * MAXIMUM_NESTED_WIDGETS * substep);
		m_OpenedWindows[i].get()->RenderWidgetTree(
			uiBatch,
			textBatch,
			glm::vec2(0.f, 0.f),
			correctBaseZlayer,
			substep
		);
	}

}

void UIManager::OpenWindow(std::unique_ptr<Window> window) {
	m_OpenedWindows.push_back(std::move(window));
}

void UIManager::MoveWindowToFront(ID windowID) {

}

void UIManager::CloseWindow(ID windowID) {

}