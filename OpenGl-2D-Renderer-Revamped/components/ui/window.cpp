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

Window::~Window() {
	for (size_t i = 0; i < m_WidgetsArray.size(); i++) {
		DEBUG_LOG("Deleting widget from window with ID [%ud]\n", GetID());
		delete m_WidgetsArray[i];
		DEBUG_LOG("Deletion successful");
	}
}