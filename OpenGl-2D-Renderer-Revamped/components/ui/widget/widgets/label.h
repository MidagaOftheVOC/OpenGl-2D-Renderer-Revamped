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

	Text* GetText() { return &m_StoredText; }

private:

	virtual void SendOwnRenderData(
		Batch* uiBatch,
		glm::vec2 absoluteParentOrigin,
		float z
	) const override;

public:

	virtual ~Label() {}

};