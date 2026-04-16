#pragma once

#include "../../common/common.h"

#include "../widget_interface.h"

class Window : public WidgetCompositionInterface{

	ID m_ID;

public:

	Window(
		ID id,
		glm::vec2 offsetRelToParent,
		glm::vec2 dimensions,
		const PaneSkin* bgSkin
	) :
		m_ID(id),
		WidgetCompositionInterface(
			offsetRelToParent,
			dimensions,
			bgSkin,
			true
		)
	{}

private:

	//	Should be empty, since Window has no graphical data beyond a background
	virtual void SendOwnRenderData(
		Batch* uiBatch,
		std::vector<TextWithZLayer>& texts,
		glm::vec2 absoluteParentOrigin,
		float z
	) const override {}

public:

	virtual ~Window() {}

};

//class Window {
//
//	ID m_ID = 0;
//	ID m_Parent = 0;
//	glm::vec2 m_WindowPosition = glm::vec2(0.f, 0.f);	//	Relative to top left corner of screen
//	glm::vec2 m_WindowDimensions = glm::vec2(200.f, 80.f);
//
//private:	// UI PRIMITIVE DATA
//
//	bool m_HasPane = false;
//	Pane m_WindowPane;
//	std::vector<UI_Primitive*> m_WidgetsArray;
//
//public:
//
//	Window() {}
//
//	Window(
//		ID _windowId
//	);
//
//	//	If set to nullptr, pane will be removed
//	void SetPane(
//		Pane&& _pane
//	);
//
//private:
//
//	WidgetWindowData GenerateWidgetData() {
//		WidgetWindowData self;
//
//		self.WindowDimensions = m_WindowDimensions;
//
//		return self;
//	}
//	
//public:
//
//	//	Also changes the pane's dimensions, if there is one
//	void SetDimensions(
//		glm::vec2 _dimensions
//	);
//
//	void SetPosition(
//		glm::vec2 _position
//	);
//
//	void AttachWidget(UI_Primitive* _widget) {
//		_widget->PostAttachment(GenerateWidgetData());
//		m_WidgetsArray.emplace_back(_widget);
//	}
//
//	const std::vector<UI_Primitive*>& GetWidgets() const { return m_WidgetsArray; }
//
//	ID GetID() const { return m_ID; }
//	const Pane& GetPane() const { return m_WindowPane; }
//	const bool HasPane() const { return m_HasPane; }
//	const glm::vec2& GetWinPosition() const { return m_WindowPosition; }
//	const glm::vec2& GetWinDimensions() const { return m_WindowDimensions; }
//
//	~Window();
//
//};
