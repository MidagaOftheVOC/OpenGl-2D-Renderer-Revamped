#include "ui_manager.h"
#include <print>

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

	if (input.AccessRecentKeystateBitmask(GLFW_MOUSE_BUTTON_LEFT)) {
		float mouseX, mouseY;
		input.GetMousePosition(mouseX, mouseY);
		glm::vec2 mousePos = glm::vec2(mouseX, mouseY);
		WidgetCompositionInterface* Target = DetectClick(glm::vec2(0.f, 0.f), mousePos);

		for (size_t i = m_OpenedWindows.size(); i-- > 0; ) {
			Window* targetWindow = m_OpenedWindows[i].get();
			WidgetCompositionInterface* windowTarget = targetWindow->DetectClick(glm::vec2(0.f, 0.f), mousePos);
			if (windowTarget) {
				MoveWindowToFront(targetWindow->GetID());
				Target = windowTarget;

				if (targetWindow == Target && input.IsHeld(GLFW_MOUSE_BUTTON_LEFT))
					targetWindow->ApplyChangeToPosition(input.GetMouseChange());

				break;
			}
		}

		if (Target && input.IsPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			Target->OnClick();
			input.SetMouseCapturedFlag();
		}

		//	TODO: IF ITS AN INPUT FIELD, SET IT AS FOCUS
	}
}

void UIManager::OpenWindow(std::unique_ptr<Window> window) {
	m_OpenedWindows.push_back(std::move(window));
}

void UIManager::MoveWindowToFront(ID windowID) {
	for (size_t i = 0; i < m_OpenedWindows.size(); i++) {
		if (m_OpenedWindows[i]->GetID() == windowID) {
			std::unique_ptr<Window> movingWindow = std::move(m_OpenedWindows[i]);
			m_OpenedWindows.erase(m_OpenedWindows.begin() + i);
			m_OpenedWindows.push_back(std::move(movingWindow));
			return;
		}
	}
}

void UIManager::CloseWindow(ID windowID) {

}