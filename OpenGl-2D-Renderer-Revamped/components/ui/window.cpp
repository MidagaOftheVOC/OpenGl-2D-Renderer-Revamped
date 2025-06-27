#include "window.h"



Window::Window(
	ID _windowId
) {
	m_ID = _windowId;
}


void Window::SetDimensions(
	glm::vec2 _dimensions
) {
	m_WindowDimensions = _dimensions;
	if (HasPane()) {
		m_WindowPane.SetDimensions(_dimensions);
	}
}


void Window::SetPosition(
	glm::vec2 _position
) {
	m_WindowPosition = _position;
}


void Window::SetPane(
	Pane&& _pane
) {
	m_HasPane = true;
	m_WindowPane = std::move(_pane);
}

