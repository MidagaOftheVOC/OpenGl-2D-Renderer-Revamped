#pragma once
#include "../ui_primitive.h"
#include "pane.h"
#include "label.h"

#include <functional>


class Button : public UI_Primitive {

	std::function<void()> m_StoredActionLambda;
	Pane m_Pane;
	Label m_Label;

public:

	Button(
		glm::vec2 relativeToWindow,
		Text label,
		Pane pane,
		std::function<void()> actionLambda
	);

public:

	virtual void DoAction();
	virtual void AppendWidgetRenderDataToArray(std::vector<float>& OUT_rects, std::vector<TextWithZLayer>& OUT_texts, float zLayer);
	virtual void PostAttachment(WidgetWindowData _data);

	const Label& GetLabel() const { return m_Label; }
	const Pane& GetPane() const { return m_Pane; }

	virtual ~Button() {}
};

