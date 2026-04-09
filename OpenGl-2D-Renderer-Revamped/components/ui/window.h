#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"

#include <vector>
#include <unordered_map>

#include "primitives/types/pane.h"

class Window {

	ID m_ID = 0;
	ID m_Parent = 0;
	glm::vec2 m_WindowPosition = glm::vec2(0.f, 0.f);	//	Relative to top left corner of screen
	glm::vec2 m_WindowDimensions = glm::vec2(200.f, 80.f);

private:	// UI PRIMITIVE DATA

	bool m_HasPane = false;
	Pane m_WindowPane;
	std::vector<UI_Primitive*> m_WidgetsArray;

public:

	Window() {}

	Window(
		ID _windowId
	);

	//	If set to nullptr, pane will be removed
	void SetPane(
		Pane&& _pane
	);

private:

	WidgetWindowData GenerateWidgetData() {
		WidgetWindowData self;

		self.WindowDimensions = m_WindowDimensions;

		return self;
	}
	
public:

	//	Also changes the pane's dimensions, if there is one
	void SetDimensions(
		glm::vec2 _dimensions
	);

	void SetPosition(
		glm::vec2 _position
	);

	void AttachWidget(UI_Primitive* _widget) {
		_widget->PostAttachment(GenerateWidgetData());
		m_WidgetsArray.emplace_back(_widget);
	}

	const std::vector<UI_Primitive*>& GetWidgets() const { return m_WidgetsArray; }

	ID GetID() const { return m_ID; }
	const Pane& GetPane() const { return m_WindowPane; }
	const bool HasPane() const { return m_HasPane; }
	const glm::vec2& GetWinPosition() const { return m_WindowPosition; }
	const glm::vec2& GetWinDimensions() const { return m_WindowDimensions; }

	~Window();

};
