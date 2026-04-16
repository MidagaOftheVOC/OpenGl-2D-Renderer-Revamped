#pragma once

#include "../widget_interface.h"

class Label : public WidgetCompositionInterface {

	Text m_StoredText;

public:
	
	Label(
		const Text& text,
		glm::vec2 offsetRelToParent,
		glm::vec2 dimensions
	) :
		m_StoredText(text),
		WidgetCompositionInterface(
			offsetRelToParent,
			dimensions,
			nullptr,
			false
		)
	{}

private:

	virtual void SendOwnRenderData(
		Batch* uiBatch,
		std::vector<TextWithZLayer>& texts,
		glm::vec2 absoluteParentOrigin,
		float z
	) const override;

public:

	virtual ~Label() {}

};