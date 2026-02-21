#include "ui_manager.h"

UIManager::UIManager(UIBatch* _uiBatch)
	: m_UIBatch(_uiBatch)
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
ID UIManager::AddWindow(
	Window _window
) {
	m_WindowArray.emplace_back(_window);
	return m_WindowArray.back().GetID();
}


void UIManager::UpdateAllBatches() {

	float PaneStep = (m_UpperBoundOfZlayerDistribution - m_LowerBoundOfZlayerDistribution) / m_WindowIDArrayForRendering.size();
	float SubStep = PaneStep / 4;

	std::vector<SpriteInformation> WidgetData_Skins;
	std::vector<float> WidgetData_Positions;
	std::vector<float> WidgetData_Dimensions;
	std::vector<float> WidgetData_zLayers;

	int PaneTotalSubsprites = 0;

	m_CachedTexts.clear();	//	reset texts for labels;


	for (size_t i = 0; i < m_WindowIDArrayForRendering.size(); i++) {
		Window* CurrWindow = GetWindowByID(m_WindowIDArrayForRendering[i]);

		if (!CurrWindow) {
			DEBUG_ASSERT(0, "Bad ID found in window render array. [%u]", m_WindowIDArrayForRendering[i]);
			continue;
		}


		float BaseWindowZLayer = m_LowerBoundOfZlayerDistribution + (i + 1) * PaneStep;

		const glm::vec2& WinPos = CurrWindow->GetWinPosition();

		//	TODO: test if Z layering is proper.
		for (int i = 0; i < 9; i++) {
			WidgetData_zLayers.emplace_back(BaseWindowZLayer);
		}
		CurrWindow->GetPane().AppendPaneBatchingData(
			WidgetData_Dimensions,
			WidgetData_Positions,
			WidgetData_Skins,
			WinPos.x,
			WinPos.y
		);

		PaneTotalSubsprites += 9;

		//	Here come loops who will take care of any primitive arrays within each window
		//	I.e. they will utilise the Micro steps adn the more precise layer distribution

		const std::vector<UI_Primitive*>& CurrWinWidgets = CurrWindow->GetWidgets();

		for (size_t i = 0; i < CurrWinWidgets.size(); i++) {
			if (Label* label = dynamic_cast<Label*>(CurrWinWidgets[i])) {
				m_CachedTexts.push_back(
					TextWithZLayer(
						label->GetText(),
						WinPos.x + label->GetXOffset(),
						WinPos.y + label->GetYOffset(),
						BaseWindowZLayer - SubStep
					)
				);
			}
			
		}
	}

	m_UIBatch->UpdateBuffers(
		WidgetData_Skins.data(),
		WidgetData_Positions.data(),
		WidgetData_Dimensions.data(),
		WidgetData_zLayers.data(),
		PaneTotalSubsprites
	);
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
	for (auto& Window : m_WindowArray) {
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
	UpdateAllBatches();
}


ID UIManager::GetNextUniqueWindowID() {
	return ++m_UniqueWindowIDCounter;
}


void UIManager::OpenWindow(
	ID _windowID,
	float _xCoord,
	float _yCoord
) {
	DEBUG_ASSERT(_xCoord >= 0 && _yCoord >= 0, "Opening window at strange coordinates. X [%.2f]\tY[%.2f].", _xCoord, _yCoord);

	Window* CurrWindow = GetWindowByID(_windowID);
	DEBUG_ASSERT(CurrWindow != nullptr, "Trying to open a window with invalid ID [%u].", _windowID);

	DEBUG_ASSERT(!IsWindowIDInRenderArray(_windowID), "Trying to open the same window ID [%u] twice.", _windowID);

	CurrWindow->SetPosition({ _xCoord, _yCoord });
	m_WindowIDArrayForRendering.emplace(m_WindowIDArrayForRendering.begin(), _windowID);
	UpdateAllBatches();
}


Window* UIManager::GetWindowByID(
	const ID _windowID
) {
	for (auto& window : m_WindowArray) {
		if (window.GetID() == _windowID) {
			return &window;
		}
	}
	return nullptr;
}


bool UIManager::IsWindowIDInRenderArray(
	const ID _windowID
) const {
	for (ID CurrWindowID : m_WindowIDArrayForRendering) {
		if (CurrWindowID == _windowID) {
			return true;
		}
	}
	return false;
}


void UIManager::CloseWindow(
	ID _windowID
) {
	for (size_t i = 0; i < m_WindowIDArrayForRendering.size(); i++) {
		if (m_WindowIDArrayForRendering[i] == _windowID) {
			m_WindowIDArrayForRendering.erase(m_WindowIDArrayForRendering.begin() + i);
			return;
		}
	}
	UpdateAllBatches();
}


void UIManager::MoveWindowToLayer(
	ID _windowID,
	size_t _layer
) {
	size_t TargetWindowIndex = GetIndexOfWindowByIDInRenderArray(_windowID);
	DEBUG_ASSERT(TargetWindowIndex != gc_ui32ErrorCode, "Trying to move window with invalid ID. [%u]", _windowID);
	DEBUG_ASSERT(_layer < m_WindowIDArrayForRendering.size(), "Attempting to move window to strange layer. [%llu]", _layer);

	if (TargetWindowIndex == _layer) return;
	
	ID TargetWindowID = m_WindowIDArrayForRendering[TargetWindowIndex];

	m_WindowIDArrayForRendering.erase(m_WindowIDArrayForRendering.begin() + TargetWindowIndex);
	m_WindowIDArrayForRendering.insert(m_WindowIDArrayForRendering.begin() + _layer, TargetWindowID);
	UpdateAllBatches();
}


size_t UIManager::GetIndexOfWindowByIDInRenderArray(
	const ID _windowID
) const {
	for (size_t i = 0; i < m_WindowIDArrayForRendering.size(); i++) {
		if (m_WindowIDArrayForRendering[i] == _windowID) {
			return i;
		}
	}
	return gc_ui32ErrorCode;
}


void UIManager::SetUiBatchPointer(UIBatch* _uiBatch) {
	m_UIBatch = _uiBatch;
}
