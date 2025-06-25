#include "ui_manager.h"
#include "../../main_renderer.h"



UIManager::UIManager(
	Renderer2D* _renderer
):
	m_MainRenderer(_renderer)
{}


void UIManager::AddWindow(
	Window _window
) {
	//m_WindowArray.emplace_back(_window);
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