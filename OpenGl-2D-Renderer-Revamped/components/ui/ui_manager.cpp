#include "ui_manager.h"
#include "../../main_renderer.h"



UIManager::UIManager(
	Renderer2D* _renderer
):
	m_MainRenderer(_renderer)
{}


Window UIManager::CreateWindow(
	glm::vec2 _windowDimensions,
	float _cornerLengthPx,
	const char* _uiSkinName
) {
	Window Result(GetNextUniqueWindowID());
	
	if(_uiSkinName) {
		Pane WindowPane = m_ThePaneFactory.CreatePane(
			_windowDimensions.x,
			_windowDimensions.y,
			_cornerLengthPx,
			_uiSkinName
		);
		Result.SetPane(std::move(WindowPane));
	}

	Result.SetDimensions(_windowDimensions);
	return Result;
}


//	TODO: track for identical IDs
void UIManager::AddWindow(
	Window _window
) {
	m_WindowArray.emplace_back(_window);
	UpdateAllBatches();
}


void UIManager::UpdateAllBatches() {

	float PaneStep = (m_UpperBoundOfZlayerDistribution - m_LowerBoundOfZlayerDistribution) / m_WindowArray.size();
	float MicroSetp = PaneStep / 4;

	std::vector<SpriteInformation> PANE_Skins;
	std::vector<float> PANE_Positions;
	std::vector<float> PANE_Dimensions;
	std::vector<float> PANE_zLayers;


	for (size_t i = 0; i < m_WindowArray.size(); i++) {
		Window& Win = m_WindowArray[i];

		//	If smaller values are closer to the screen AND the first window is the one closes to us
		//	AND the LowerBound is the one whose absolute value is lower than the other bound
		//	this should be the formula.
		float PaneZLayer = i * PaneStep + m_LowerBoundOfZlayerDistribution;

		const glm::vec2& WinPos = Win.GetWinPosition();

		//	TODO: test if Z layering is proper.
		PANE_zLayers.emplace_back(PaneZLayer);
		Win.GetPane().AppendPaneBatchingData(
			PANE_Dimensions,
			PANE_Positions,
			PANE_Skins,
			WinPos.x,
			WinPos.y
		);


		//	Here come loops who will take care of any primitive arrays within each window
		//	I.e. they will utilise the Micro steps adn the more precise layer distribution


	}


	m_PaneBatch.UpdateBuffers(
		PANE_Skins.data(),
		PANE_Positions.data(),
		PANE_Dimensions.data(),
		PANE_zLayers.data(),
		m_WindowArray.size() * 9
	);
}


void UIManager::RenderAllActiveWindows() {
	m_MainRenderer->Draw(&m_PaneBatch, 0, nullptr);
}


void UIManager::SetDistributionBounds(
	float _low, 
	float _high
) {
	m_UpperBoundOfZlayerDistribution = _high;
	m_LowerBoundOfZlayerDistribution = _low;
}


size_t UIManager::CountAllWindowsWithBackgrounds() const {
	size_t Count = 0;
	for (auto Window : m_WindowArray) {
		if (Window.HasPane()) Count++;
	}
	return Count;
}



void UIManager::RemoveWindow(
	ID _windowID
) {
	for (size_t i = 0; i < m_WindowArray.size(); i++) {
		if (m_WindowArray[i].GetID() == _windowID) {
			m_WindowArray.erase(m_WindowArray.begin() + i);
			return;
		}
	}
}


ID UIManager::GetNextUniqueWindowID() {
	return m_UniqueWindowIDCounter++;
}
