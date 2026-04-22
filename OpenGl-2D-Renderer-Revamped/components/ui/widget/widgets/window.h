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
		const BackgroundSkin* bgSkin
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
		glm::vec2 absoluteParentOrigin,
		float z
	) const override {}

public:

	ID GetID() const { return m_ID; }

	virtual ~Window() {}

};